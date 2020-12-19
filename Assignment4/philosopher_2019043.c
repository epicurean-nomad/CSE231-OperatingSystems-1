/*
 * Name : Harsh Kumar
 * Roll No : 2019043 
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define k 5

struct semaphore {
    // value: stores the semaphore value
    int value;

    // pending: variable used for conditional waiting
    int pending;

    // lock: mutex lock for semaphore modification
    pthread_mutex_t lock;

    // cond: conditional lock for waiting threads
    pthread_cond_t cond;
};

int sem_init(struct semaphore *sem, int value) {
    /*
     * Initialize the semaphore with value
     */
    sem->value = value;
    pthread_mutex_init(&(sem->lock), NULL);
    pthread_cond_init(&(sem->cond), NULL);
    sem->pending = 0;
}

void sem_wait(struct semaphore *sem) {
    /*
     * Blocking variant wait() 
     * Takes lock on semaphore
     */
    pthread_mutex_lock(&(sem->lock));

    sem->value--;
    if(sem->value < 0) {
        // Wait while resource gets free
        do {
            pthread_cond_wait(&(sem->cond), &(sem->lock));
        }
        while(sem->pending < 1);

        // Take the available resource
        sem->pending --;
    }

    // Release lock
    pthread_mutex_unlock(&(sem->lock));
}

int sem_trywait(struct semaphore *sem) {
    /*
     * Non blocking variant of wait()
     * Takes lock on semaphore
     */

    // Returns -1, if locking fails
    int res = pthread_mutex_trylock(&(sem->lock));
    if(res != 0) 
        return -1;
    
    if(sem->value <= 0) {
        // Cannot take lock
        res = -1;
    }
    else {
        // Lock successful
        sem->value--;
        res = 0;
    }

    pthread_mutex_unlock(&(sem->lock));

    return res;
}

void sem_post(struct semaphore *sem) {
    /*
     * signal() equivalent, increments semaphore value
     */

    // Take lock on semaphore
    pthread_mutex_lock(&(sem->lock));

    // Increase semaphore value
    sem->value++;
    if(sem->value <= 0) {
        // Signal waiting threads to start
        sem->pending ++;
        pthread_cond_signal(&(sem->cond));
    }

    // Release semaphore mutex lock
    pthread_mutex_unlock(&(sem->lock));
}

void sem_destroy(struct semaphore *sem) {
    /*
     * Destroy the semaphore
     */
    pthread_mutex_destroy(&(sem->lock));
    pthread_cond_destroy(&(sem->cond));
}

void print_sem_value(struct semaphore *sem) {
    /*
     * Print semaphore value, for debuggin purpose
     */
    pthread_mutex_lock(&(sem->lock));
    printf("%d\n", sem->value);
    pthread_mutex_unlock(&(sem->lock));
}

struct semaphore chopstick[k];
struct semaphore bowls;

pthread_t philosopher[k];

void *phil_eats(void *args) {
    /*
     * The philosopher eats in a while loop
     * Grabs left chopstick, then right chopstick, then a bowl
     * Eats and then releases resources
     */
	int i = *((int *)args);

	while(1) {
		sem_wait(&chopstick[i]);
		if(sem_trywait(&chopstick[(i+1) % k]) < 0) {
			sem_post(&chopstick[i]);
			continue;
		}
        if(sem_trywait(&bowls) < 0) {
			sem_post(&chopstick[i]);
            sem_post(&chopstick[(i+1) % k]);
			continue;
		}
		printf("Philosopher <%d> eats using forks <%d> and <%d>\n", i, i, (i+1)%k);

		sem_post(&chopstick[i]);
		sem_post(&chopstick[(i+1) % k]);
        sem_post(&bowls);
	}

	pthread_exit(NULL);
}

int main(void) {
    // Thread id for passing in the thread
    int *tmp = (int *) malloc(k * sizeof(int));

    // Initialize semaphores
	for(int i=0; i<k; ++i)
		sem_init(&chopstick[i], 1);
    sem_init(&bowls, 2);

    // Creating threads
	for(int i=0; i<k; ++i) {
		*(tmp + i) = i;
		pthread_create(&philosopher[i], NULL, phil_eats, (tmp+i));
	}

    // Waiting for the threads to complete
	for(int i=0; i<k; ++i)
		pthread_join(philosopher[i], NULL);

    for(int i=0; i<k; ++i)
		sem_destroy(&chopstick[i]);
    
    free(tmp);

	printf("Everyone has finished eating, without deadlocks!\n");
	return 0;
}