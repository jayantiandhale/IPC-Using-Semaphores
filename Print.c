/*
Print.c: Prints all available records from shared memory to standard ouptut
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
          
        /*Get id of shared memory segment id where we stored data*/
        sharedMem_id1=GetSharedMemoryId(KEY1, SEGSIZE1,0);
        sharedMem_id2=GetSharedMemoryId(KEY2, SEGSIZE2,0);
       
        /*attach this shared memory to pointer */
        stdinfoptr=(struct Student_Info *)shmat(sharedMem_id1,0,0);
        if(stdinfoptr <= (struct Student_Info *)(0)){
                perror("Print: shmat while attaching shared memory with student info failed");
                exit(2);
        }

        read_count_ptr=(int *)shmat(sharedMem_id2,0,0);
        if(read_count_ptr == ((int *)-1)){
                perror("Print: shmat while attaching shared memory with read_count failed");
                exit(2);
        }
        /* get id of semaphore set associated with SEMA_KEY*/
        semaphore_set=semget(SEMA_KEY, 0, 0);
      
	/*Acquire resources to the shared memory for read_count*/
	Wait(semaphore_set,1);
	/*Increment read_count shared memory variable by 1*/
	*read_count_ptr = *read_count_ptr + 1;
	/*If it's the only reader to read from shared memory at present*/
	if(*read_count_ptr == 1){
	        Wait(semaphore_set,0);
	}
	/*Release resources holding shared memory for read_count*/
	Signal(semaphore_set,1);
	rec_found=0;
	/*Search for a record for student*/
       	while(strlen(stdinfoptr->StudentId) != 0){
        	rec_found=rec_found+1;
                printf("\n\n StudentId:  %s\n Name: %s\n address: %s\n telephone: %s\n who modified: %s\n", stdinfoptr->StudentId,stdinfoptr->Name,stdinfoptr->address, stdinfoptr->telephone, stdinfoptr->whoModified);
        		       
		/*Increment struct pointer to next student record in shared memory*/
                stdinfoptr++;
		wait(5);
        }
	/*Print error message if record for given student id is not present in shared memory*/
         if(rec_found<1){
         	printf("\nSorry!!Student information NOT found in shared memory\n");
         }
	/*Acquire resources to shared memory for read count to decrement the value of read count*/
	Wait(semaphore_set,1);
	*read_count_ptr = *read_count_ptr - 1;
	/*Release resources holding shared memory for read_count*/
	if(*read_count_ptr == 0){
	        Signal(semaphore_set,0);
	}
	Signal(semaphore_set,1);
        exit(0);
}
                         

