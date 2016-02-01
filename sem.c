/*
sem.c: Implements all functions defined in sem.h
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "sem.h"

/*Request Resource*/
void Wait(int semaphore_id, int no_semaphore)
{
	struct sembuf sem_buffer;
	sem_buffer.sem_num = no_semaphore;
	sem_buffer.sem_op = -1;	//aquire resource
	sem_buffer.sem_flg = 0;
	semop(semaphore_id, &sem_buffer, 1);
}
/*Release the resource*/
void Signal(int semaphore_id, int no_semaphore)
{
	struct sembuf sem_buffer;
        sem_buffer.sem_num = no_semaphore;
        sem_buffer.sem_op = +1;	//Release resource
        sem_buffer.sem_flg = 0;
	semop(semaphore_id, &sem_buffer, 1);
}
/*Get existing set of semaphores with key key_t*/
int GetSemaphs(key_t key, int no_semaphore)
{
	int semaphore_id, count;
	if((semaphore_id=semget(key, no_semaphore, IPC_CREAT|0666))!=-1){
		for(count=0; count<no_semaphore; count++){
			/* Unlock semaphores in semaphore set with key*/
			Signal(semaphore_id,count);	 
		}
	}
	return semaphore_id;
} 
int GetSharedMemoryId(key_t KEY,int size,int flag){
        int sharedMem_id;
        sharedMem_id=shmget(KEY, size,flag);
        if(sharedMem_id<0){
                perror("shmget: getting shared memory failed");
                exit(1);
        }

}
	

