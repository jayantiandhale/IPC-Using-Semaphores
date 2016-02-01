/*
Load: Reads the initial database from a file and load it into shared memory.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "sem.h"
#include <string.h>
#include <assert.h>

main()
{
	int i, sharedMem_id1, sharedMem_id2,semaphore_set, *read_count_ptr;
	
	struct Student_Info *stdinfoptr;
	struct shmid_ds * shmid_ds1;
        struct shmid_ds * shmid_ds2;
	
	FILE *fp;
	
        char Name[40];
        char StudentId[20];
        char address[50];
        char telephone[15];
	
	sharedMem_id1 = GetSharedMemoryId(KEY1, SEGSIZE1, IPC_CREAT|0666);
	sharedMem_id2 = GetSharedMemoryId(KEY2, SEGSIZE2, IPC_CREAT|0666);
	
	/*Attach shared memory 1 to the student record space*/
	stdinfoptr=(struct Student_Info *)shmat(sharedMem_id1,0,0);
	if(stdinfoptr<=(struct Student_Info *)(0)){
		perror("shmat: joining address space of student info with shared memory failed");
		exit(2);
	}

	 /*Attach shared memory 2 to the read_count variable*/
	read_count_ptr = shmat(sharedMem_id2,NULL,0);
	if(read_count_ptr==((int *)-1)){
		perror("shmat: joining address space of read_count with shared memory failed");
		exit(2);
	}

	/* get a set of NUM_SEMAPHS i.e. 2 semaphores*/
	semaphore_set = GetSemaphs(SEMA_KEY, NUM_SEMAPHS);
  	if ((semaphore_set < 0)){
    		perror("semget: getting semaphores failed");
    		exit(3);
  	}
	/*Acquire the resource to shared memory of read count */	
	Wait(semaphore_set,0);
	/*Initialize the read_count to 0 to ensure no one reads while loading information*/
	*read_count_ptr=0;
	/*open input file Student_Info*/
	fp = fopen("Saved_Student_Info.dat","r");
	if(fp == NULL){
		fprintf(stderr, "Can't open input file Student_Info!\n");
		exit(4);
	}

	/*Read data from input file and store in shared memory pointed by pointer stdinfoptr*/
	while(fgets(Name,150,fp) && fgets(StudentId,150,fp) && fgets(address,150,fp) && fgets(telephone,150,fp)){	         
		strcpy(stdinfoptr->Name,Name);
		strcpy(stdinfoptr->StudentId,StudentId );
		strcpy(stdinfoptr->address,address);
	        strcpy(stdinfoptr->telephone,telephone );
                strcpy(stdinfoptr->whoModified, " ");
	
		stdinfoptr++;
	}
	/*If the reading from file stops because of file error then print the error messsage*/
	if(ferror(fp)){
		printf("Error while reading file");
		exit(5);
	}
	/*Print success message if there is no error while loading*/
	printf("\nCongratulations!!Successful Load from file to shared memory!!\n");
	sleep(10);
	/*Release the resource to shared memory*/
	Signal(semaphore_set,0);
}
