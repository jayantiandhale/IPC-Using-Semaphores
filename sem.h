#define KEY1 ((key_t)(122254))
#define KEY2 ((key_t)(222254))
#define NUM_SEMAPHS 2
#define SEMA_KEY ((key_t)(2254))
#define SEGSIZE1 (sizeof(struct Student_Info) * (20))
#define SEGSIZE2 (sizeof(int))

struct Student_Info{
	char Name[40];
	char StudentId[20];
	char address[50];
	char telephone[15];
	char whoModified[10];	
};
void Wait(int semaphore, int no_semaphore);
void Signal(int semaphore, int no_semaphore);
int GetSemaphs(key_t k, int no_semaphore);
int GetSharedMemoryId(key_t KEY,int size_mem_segment,int flag);
