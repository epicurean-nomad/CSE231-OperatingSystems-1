#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <wait.h>

#define smem_bak1 "/hadron4"
#define smem_bak2 "/hadron3"
#define sema_name "sema43"
#define smem_size 500

void check_error(int return_val, char *msg) {
	if(return_val < 0) {
		perror(msg);
		exit(return_val);
	}
}

void check_null(void *ptr, char *msg) {
	if(ptr == NULL) {
		perror(msg);
		exit(-1);
	}
}

int main(void) {
	pid_t pid;
	pid = fork();
	check_error(pid, "fork");

	// Open shared memory
	int fd1 = shm_open(smem_bak1, O_RDWR | O_CREAT, 0644);
	check_error(fd1, "shm_open");
	int fd2 = shm_open(smem_bak2, O_RDWR | O_CREAT, 0644);
	check_error(fd2, "shm_open");

	// Map the memory
	caddr_t mem_ptr1 = mmap(NULL, smem_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);
	check_null(mem_ptr1, "mmap");
	caddr_t mem_ptr2 = mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
	check_null(mem_ptr2, "mmap");

	// Open a named Semaphore
	sem_t* sem_ptr = sem_open(sema_name, O_CREAT, 0644, 1);
	check_null(sem_ptr, "sem_open");
	
	printf("Check1\n");

	sem_wait(sem_ptr);
	printf("Check2\n");
	if(*((char *)mem_ptr1) != '\0' || *((int *)mem_ptr2) != 0){
		memset(mem_ptr1, 0, smem_size);
		memset(mem_ptr2, 0, 4);
	}
	sem_post(sem_ptr);

	for(int i=1; i<=5; ++i) {
		if(pid == 0) {
			// Child Process
			sem_wait(sem_ptr);
			printf("Child read%d-> %s; %d\n", i, (char *)mem_ptr1, *((int *)mem_ptr2));
			sprintf(mem_ptr1, "Msg%d from Child", i);
			
			// Release the lock on semaphore
			sem_post(sem_ptr);
		}
		else {
			// Parent Process
			sem_wait(sem_ptr);
			printf("Parent read%d-> %s; %d\n", i, (char *)mem_ptr1, *((int *)mem_ptr2));
			sprintf(mem_ptr1, "Msg%d from Parent", i);
			
			// Release the lock on semaphore
			sem_post(sem_ptr);
		}
	}

	// Cleanup
	munmap(mem_ptr1, smem_size);
	munmap(mem_ptr2, 4);
	close(fd1);
	close(fd2);
	sem_close(sem_ptr);

	if(pid == 0) {
		exit(0);
	}
	wait(NULL);
}
