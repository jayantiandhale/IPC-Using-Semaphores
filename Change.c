/*
Change.c: Allows student advisor to change the student info in shared memory
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
	int count, sharedMem_id1, sharedMem_id2,rec_found;
	struct Student_Info *stdinfoptr;
	int *read_count_ptr;
	int semaphore_set;
	char StudentId[20];
	char Name[40];
	char address[50];
	char telephone[15];
	char password[10];
  	char validUser[10]="000\n";	
	/*Get id of shared memory segment id where we stored data*/
	sharedMem_id1=GetSharedMemoryId(KEY1, SEGSIZE1,0);
	sharedMem_id2=GetSharedMemoryId(KEY2, SEGSIZE2,0);
	
	/*attach this shared memory to pointer */
	stdinfoptr=(struct Student_Info *)shmat(sharedMem_id1,0,0);
	if(stdinfoptr <= (struct Student_Info *)(0)){
		perror("change: shmat while attaching shared memory with student info failed");
		exit(2);
	}
	
	read_count_ptr=(int *)shmat(sharedMem_id2,0,0);
	if(read_count_ptr == ((int *)-1)){
		perror("change: shmat while attaching shared memory with read_count failed");
		exit(2);
	}
	/* get id of semaphore set associated with SEMA_KEY*/
	semaphore_set=semget(SEMA_KEY, 0, 0);
	/*Verify if user is "Advisor"*/
	printf("\nPlease, enter your password:");
	fgets(password,100,stdin);
	rec_found=0;
	/*if verified user, then allow him to enter student id and and update its information in shared memory if found*/
	if(strcmp(password,validUser)==0){
		printf("Welcome dear Advisor!!");
		printf("\nEnter the student id to modify his/her record: ");
		fgets(StudentId,sizeof(StudentId),stdin);
		/*Acquire the shared memory to update*/
		Wait(semaphore_set,0);
		while(strlen(stdinfoptr->StudentId) != 0){
			if(strcmp(stdinfoptr->StudentId,StudentId)==0){
				rec_found=rec_found+1;
				/*Get new values for Name,address, telephone fields from advisor*/
				printf("\nYeah! Record is present for student id %s", StudentId);
				printf("\nAvailable information for %s\n Name: %s\n address: %s\n telephone: %s\n who modified: %s\n", stdinfoptr->StudentId,stdinfoptr->Name,stdinfoptr->address, stdinfoptr->telephone, stdinfoptr->whoModified);
				printf("\nPlease enter updated information for this student:");
				printf("\nName: ");
		                fgets(Name,sizeof(Name),stdin);
				printf("Address: ");
		                fgets(address,sizeof(address),stdin);
                		printf("Telephone no:");
		                fgets(telephone,sizeof(telephone),stdin);
				/*Update new values for corresponding student id in shared memory*/
				strcpy(stdinfoptr->Name, Name);
				strcpy(stdinfoptr->address, address);
				strcpy(stdinfoptr->telephone, telephone);
				strcpy(stdinfoptr->whoModified,(getpwuid(getuid()))->pw_name); 
			
				/*Indicate the successful update to user*/
				printf("\n\t Congratulations!! Record updated in shared memory for student id %s ", StudentId);
				printf("\t Updated information for %s\t Name: %s\t address: %s\t telephone: %s\t who modified: %s\n", stdinfoptr->StudentId,stdinfoptr->Name,stdinfoptr->address, stdinfoptr->telephone, stdinfoptr->whoModified);
				break;
			}
			/*Search for next student id if required student id is not found!*/
			stdinfoptr++;
		}
		if(rec_found<1){
			printf("\nSorry!!Student information NOT found for student id %s\n",StudentId);
		}	
	
	}
	/*If user is not an Advisor print invalid user */
	else{
		printf("Sorry!Invalid user password!\n");
		exit(3);
	} 		 
	sleep(10);
	/*Release resources to shared memory after successful update*/ 
	Signal(semaphore_set,0);
	exit(0);
}	
