/* Name: Harsh Kumar
   Roll_Number: 2019043 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define __NR_identity 440

long rtnice(long pid, long long softruntime) {
	return syscall(__NR_identity, pid, softruntime);
}

void busy_wait() {
	for(long long i=0; i<9999999999L; ++i);
}

void fork_and_test(long long softruntime) {
	pid_t pid = fork();
	if(pid < 0) {
		perror("Forking Failed");
	}
	else if((long)pid == 0) {
		pid_t pid_new;
		char *endptr;
		clock_t t;

		pid_new = getpid();

		if(rtnice((long)pid_new, softruntime)) {
			perror("Test Failed");
			return;
		}

		printf("\nRunning with realtime requirements\n");
		t = clock();

		busy_wait();

		t = clock() - t;
		printf("Time taken: %f\n", ((double)t)/CLOCKS_PER_SEC);

		exit(0);
	}
	else {
		wait(NULL);
		printf("\nRunning without realtime requirements\n");
		clock_t t = clock();

		busy_wait();

		t = clock() - t;
		printf("Time taken: %f\n", ((double)t)/CLOCKS_PER_SEC);
	}
}

int main(int argc, char *argv[]) {
	long long softruntime;
	printf("Enter softruntime value: ");
	scanf("%lld", &softruntime);
	fork_and_test(softruntime);
	return 0;
}