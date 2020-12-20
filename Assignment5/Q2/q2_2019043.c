/*
 * Name: Harsh Kumar
 * Roll No; 2019043
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <error.h>
#include <errno.h>
#include <unistd.h>

#define MAX_LINE_SIZE 500

char *data = NULL, *add = NULL;
int max_size = 0, size = 0;
FILE *fptr = NULL;
int fd;

void add_data(char *str) {
    if(data == NULL) {
        data = malloc(100);
        max_size = 100;
        size = 0;
    }
    else if(max_size == size) {
        data = realloc(data, 2*max_size);
        max_size = 2*max_size;
    }

    if(max_size > size + strlen(str)) {
        data = realloc(data, max_size + strlen(str));
        max_size = max_size + strlen(str);
    }

    strcat(data, str);
}

void delete_data() {
    if(data != NULL)
        free(data);
}

void open_file(char *filepath) {
    fd = open(filepath, O_RDWR | O_CREAT);

    if(fd < 0) {
        perror("open");
        exit(-1);
    }

    fptr = fdopen(fd, "rw");

    if(fptr == NULL) {
        perror("fopen");
        exit(-1);
    }

    if(flock(fd, LOCK_EX | LOCK_NB) < 0) {
        if(errno == EWOULDBLOCK) {
            printf("Warning! This file is being edited by some other program.\n");
        }
        else {
            perror("flock");
            exit(-1);
        }
    }
    
    char *buf = NULL;
    size_t len = 0;
    while(getline(&buf, &len, fptr) != -1) {
        add_data(buf);
    }

    if(buf)
        free(buf);
}

void close_file() {
    if(add && add[0] != 10)
        write(fd, add, strlen(add));

    if(flock(fd, LOCK_UN) < 0) {
        perror("flock");
        exit(-1);
    }

    if(fptr != NULL)
        fclose(fptr);
    max_size = 0;
    size = 0;
}


int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Too few arguments passes! Pass a filepath to open it.");
        exit(-1);
    }

    if(argc > 2) {
        fprintf(stderr, "Too many arguments passed! Pass single filepath to open it.");
        exit(-1);
    }

    open_file(argv[1]);
    if(data != NULL)
        printf("File contents: \n%s\n", data);

    // Max one line can be appended
    add = malloc(MAX_LINE_SIZE);
    size_t size = 0;
    printf("Enter a line to be appended to file (enter to skip): ");
    getline(&add, &size, stdin);

    close_file(); 
    free(add);   
}