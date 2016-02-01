/*
Query.c: Fetch record for specific student and display it.
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
        char StudentId[20];
        
        /*Get id of shared memory segment id where we stored data*/
        sharedMem_id1=GetSharedMemoryId(KEY1, SEGSIZE1,0);
        sharedMem_id2=GetSharedMemoryId(KEY2, SEGSIZE2,0);
        
        /*attach this shared memory to pointer */
        stdinfoptr=(struct Student_Info *)shmat(sharedMem_id1,0,0);
        if(stdinfoptr <= (struct Student_Info *)(0)){
                perror("Query: shmat while attaching shared memory with student info failed");
                exit(2);
        }

        read_count_ptr=(int *)shmat(sharedMem_id2,0,0);
        if(read_count_ptr == ((int *)-1)){
                perror("Query: shmat while attaching shared memory with read_count failed");
                exit(2);
        }
        /* get id of semaphore set associated with SEMA_KEY*/
        semaphore_set=semget(SEMA_KEY, 0, 0);
        
	printf("Enter the student id to search: ");
        fgets(StudentId,sizeof(StudentId),stdin);
	/*Acquire the shared memory resource to update read count*/
	Wait(semaphore_set,1);
	*read_count_ptr = *read_count_ptr + 1;
	if(*read_count_ptr == 1){
	        Wait(semaphore_set,0);
	}
	/*Release the shared memory resource for read_count*/
	Signal(semaphore_set,1);
	rec_found=0;
	/*Search and print the student record from shared memory for student info*/
       	while(strlen(stdinfoptr->StudentId) != 0){
        	if(strcmp(stdinfoptr->StudentId,StudentId)==0){
                	rec_found=rec_found+1;
                        printf("\n \tCongratulations! Record found for student id %s", StudentId);
                        printf("\n \tAvailable information for student id %s\n \tName: %s\n \taddress: %s\n \ttelephone: %s\n \twho modified: %s\n", stdinfoptr->StudentId,stdinfoptr->Name,stdinfoptr->address, stdinfoptr->telephone, stdinfoptr->whoModified);
               		break;
		 }
         stdinfoptr++;
         }
	/*Print error message if student record is not available for given student id*/
         if(rec_found<1){
         	printf("\nSorry! Student information NOT found for student id %s\n",StudentId);
         }
	/*Acquire shared memory for read_count, Decrement the read_count pointer and release the resource from shared memory */
	Wait(semaphore_set,1);
	*read_count_ptr = *read_count_ptr - 1;
	if(*read_count_ptr == 0){
	        Signal(semaphore_set,0);
	}
	Signal(semaphore_set,1);
        exit(0);
}
                         

