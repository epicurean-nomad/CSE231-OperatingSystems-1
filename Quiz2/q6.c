#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define phil_count 5

sem_t chopstick[phil_count];

void *phil_eats(void *input) {
	int done = 0;
	int i = *((int *)input);

	while(!done) {
		sem_wait(&chopstick[i]);
		if(sem_trywait(&chopstick[(i+1) % phil_count]) < 0) {
			sem_post(&chopstick[i]);
			continue;
		}
		printf("Phil%d is eating now...\n", i);

		sem_post(&chopstick[i]);
		sem_post(&chopstick[(i+1) % phil_count]);
		done = 1;
	}

	pthread_exit(NULL);
}

int main(void) {
	int *tmp = (int *) malloc(phil_count * sizeof(int));

	for(int i=0; i<phil_count; ++i)
		sem_init(&chopstick[i], 0, 1);

	pthread_t t[phil_count];
	for(int i=0; i<phil_count; ++i) {
		*(tmp + i) = i;
		pthread_create(&t[i], NULL, phil_eats, (tmp+i));
	}

	for(int i=0; i<phil_count; ++i)
		pthread_join(t[i], NULL);

	printf("Everyone has finished eating, without deadlocks!\n");
	return 0;
}
