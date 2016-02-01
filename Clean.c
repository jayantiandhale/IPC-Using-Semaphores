/*
Clean.c: Copies all data from shared memory to Saved_Student_Info.dat and then 
deletes the shared memmory segments and semaphores.
*/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sem.h>
#include <pwd.h>
#include "sem.h"
#include <stdlib.h>
main()
{
        int sharedMem_id1, sharedMem_id2,rec_found;
        struct Student_Info *stdinfoptr;
        int *read_count_ptr;
        int semaphore_set;
        
	FILE *fp;

        struct shmid_ds * shmid_ds1;
        struct shmid_ds * shmid_ds2;
        
        /*Get id of shared memory segment id where we stored data*/
        sharedMem_id1=GetSharedMemoryId(KEY1, SEGSIZE1,0);
        sharedMem_id2=GetSharedMemoryId(KEY2, SEGSIZE2,0);
       
	 /*attach this shared memory to pointer */
        stdinfoptr=(struct Student_Info *)shmat(sharedMem_id1,0,0);
        if(stdinfoptr <= (struct Student_Info *)(0)){
                perror("clean: shmat while attaching shared memory with student info failed");
                exit(2);
        }

        read_count_ptr=(int *)shmat(sharedMem_id2,0,0);
        if(read_count_ptr == ((int *)-1)){
                perror("clean: shmat while attaching shared memory with read_count failed");
                exit(2);
        }
        /* get id of semaphore set associated with SEMA_KEY*/
        semaphore_set=semget(SEMA_KEY, 0, 0);
    	/*Acquire the resource to shared memory*/
	Wait(semaphore_set,0);

	/*create and open file to write in*/
	fp=fopen("Saved_Student_Info.dat","w");
        if(fp == NULL){
                fprintf(stderr, "Can't open output file!\n");
                exit(3);
        }
	rec_found=0;

	/*write data from shared memory for student info to output file*/
       	while(strlen(stdinfoptr->StudentId) != 0){
               	rec_found=rec_found+1;
                fputs(stdinfoptr->StudentId,fp);
		fputs(stdinfoptr->Name,fp);
		fputs(stdinfoptr->address,fp);
		fputs(stdinfoptr->telephone,fp);
         	stdinfoptr++;
        }
	/*If no data is present in shared memory, let user know about it*/
         if(rec_found<1){
         	printf("\nNo data in shared memory!\n");
        }
	if(ferror(fp)){
		printf("\nError while writing to file!");
		exit(4);
	}
	/*Close output file*/
	fclose(fp);
	/*If no error acknowledge successful write to ouput file*/
	printf("\nSuccessfully wrote contents of shared memory to output file!\n");	
	/*Detach shared memory segments*/
        shmdt((char *)stdinfoptr);
        shmdt((int *)read_count_ptr);

        /*destroy shared memory segments*/
        shmctl(sharedMem_id1, IPC_RMID, shmid_ds1);
        shmctl(sharedMem_id2, IPC_RMID, shmid_ds2);
	
	sleep(10);
	/*Release resource to shared memory*/
	Signal(semaphore_set,0);
        /*remove semaphore set*/
        semctl(semaphore_set,0,IPC_RMID);
	/*Aknowledge successful clean */
	printf("\nSuccessful Clean!\n");
	exit(0);
}
                         

