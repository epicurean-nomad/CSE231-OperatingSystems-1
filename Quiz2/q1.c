#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

int main(int argc, char *argv[]) {
    pid_t pid;
    int p1[2], p2[2];

    // p1[1] -> parent writes
    // p1[0] -> child reads
    if(pipe(p1)) {
        perror("Pipe creation unsuccessful");
        exit(1);
    }

    // p2[1] -> child writes
    // p2[0] -> parent reads
    if(pipe(p2)) {
        perror("Pipe creation unsuccessful");
        exit(1);
    }

    pid = fork();
    if(pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    else if(pid > 0) {
        // Parent Process

        // Close unused pipe file descriptors
        close(p1[0]);
        close(p2[1]);

        printf("Enter a string: ");
        char str[500];
        fgets(str, 500, stdin);
        
        write(p1[1], str, strlen(str));

        read(p2[0], str, 500);
        printf("Message from Child: %s", str);

        wait(NULL);
    }
    else {
        // Child Process

        // Close unused pipe file descriptors
        close(p2[0]);
        close(p1[1]);

        char str[500];
        read(p1[0], str, 500);
        
        for(int i=0; i<strlen(str); ++i) {
            if(str[i] >= 97 && str[i] <= 122) {
                str[i] -= 32;
            }
        }

        write(p2[1], str, strlen(str));
        exit(0);
    }
    return 0;
}