#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <wait.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define sema_name1 "sema4"
#define sema_name2 "sema3"
#define smem_size 500

int shmid1, shmid2;
char *shm1, *shm2;

void check_error(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

int main(void) {
	pid_t pid;
	pid = fork();
	if(pid < 0)
		check_error("fork");

	sem_t *sem1 = sem_open(sema_name1, O_CREAT, 0644, 1);
	if(sem1 == SEM_FAILED)
		check_error("sem_open");
	
	sem_t *sem2 = sem_open(sema_name2, O_CREAT, 0644, 1);
	if(sem2 == SEM_FAILED)
		check_error("sem_open");

	shmid1 = shmget(2009, smem_size, 0666 | IPC_CREAT);
	if(shmid1 == -1)
		check_error("shmget");
	shm1 = shmat(shmid1, 0, 0);
	if(shm1 == (void *)-1)
		check_error("shmget");

	shmid2 = shmget(2010, 4, 0666 | IPC_CREAT);
	if(shmid2 == -1)
		check_error("shmget");
	shm2 = shmat(shmid2, 0, 0);
	if(shm2 == (void *)-1)
		check_error("shmget");

	for(int i=0; i<5; ++i) {
		if(pid == 0) {
			// Child Proess
			sem_wait(sem1);	
			printf("Child reads: %s;", (char *)shm1);
			sprintf(shm1, "Child writes %d", i);
			sem_post(sem1);

			sem_wait(sem2);
			printf("%d\n", *((int *)shm2));
			int *int_ptr = (int *)shm2;
			*int_ptr = i;
			sem_post(sem2);
		}
		else {
			// Parent Process
			sem_wait(sem1);	
			printf("Parent reads: %s;", (char *)shm1);
			sprintf(shm1, "Child writes %d", i);
			sem_post(sem1);

			sem_wait(sem2);
			printf("%d\n", *((int *)shm2));
			int *int_ptr = (int *)shm2;
			*int_ptr = i;
			sem_post(sem2);
		}
	}

	sem_destroy(sem1);
	sem_destroy(sem2);

	shmdt(shm1);
	shmctl(shmid1, IPC_RMID, NULL);
	shmdt(shm1);
	shmctl(shmid1, IPC_RMID, NULL);

	if(pid == 0)
		exit(0);
	wait(NULL);
	return 0;
}
