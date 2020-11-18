#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define shm_size 8
#define total_thread 5
// writer thread range -> (0, total_thread-1)
#define writer_thread_no 1

int shmid, read_count = 0, cnt = 0;
char *shm;
sem_t rw_mutex, mutex;

void* reader_thread(void* arg) {
	//wait
	sem_wait(&mutex);
	read_count++;
	if(read_count == 1)
		sem_wait(&rw_mutex);
	
	sem_post(&mutex);

	printf("Reader starts reading...\n");

	//critical section
	printf("Reader reads <%s>\n", shm);
	//End of critical section

	printf("Reader ends reading...\n");

	sem_wait(&mutex);
	read_count--;
	if(read_count == 0)
		sem_post(&rw_mutex);
	sem_post(&mutex);

	pthread_exit(NULL);
}

void* writer_thread(void* arg) {
	//wait
	sem_wait(&rw_mutex);
	printf("Writer started writing..\n");

	sprintf(shm, "W%d", ++cnt);
	
	printf("Writer Finished writing...\n");
	sem_post(&rw_mutex);

	pthread_exit(NULL);
}

int main() {
	sem_init(&mutex, 0, 1);
	sem_init(&rw_mutex, 0, 1);

	shmid = shmget(2009, shm_size, 0666 | IPC_CREAT);
	shm = shmat(shmid, 0, 0);

	
	pthread_t t[total_thread];
	// Initializing writer thread
	for(int i=0; i<5; ++i) {
		if(i == writer_thread_no)
			pthread_create(&t[i], NULL, writer_thread, NULL);
		else	
			pthread_create(&t[i], NULL, reader_thread, NULL);
	}

	for(int i=0; i<5; ++i)
		pthread_join(t[i], NULL);

	sem_destroy(&mutex);
	shmdt(shm);
	shmctl(shmid, IPC_RMID, NULL);

	return 0;
}