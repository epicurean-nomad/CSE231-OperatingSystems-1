#include <stdlib.h>
#include <stdio.h>
#include <syscall.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

void printSize(char *str, int size) {
    syscall(1, STDOUT_FILENO, str, size);
}

void print(char *str) {
    int len = strlen(str);
    printSize(str, len);
}

void printInt(int input) {
    char string[20];
    sprintf(string, "%d", input);
    print(string);
}

void printFloat(float input) {
    char string[100];
    sprintf(string, "%f", input);
    print(string);
}

struct student {
    unsigned int id;
    char section;
    int marks[4];
};

int parseStudent(char *str, struct student *s) {
    char *pch;
    pch = strtok(str, ",");

    if(pch == NULL)
        return -1;
    
    s->id = atoi(pch);
    pch = strtok(NULL, ",");

    if(pch == NULL)
        return -1;

    s->section = pch[0];

    for(int i=0; i<3; ++i) {
        pch = strtok(NULL, ",");
        if(pch == NULL)
            return -1;
        s->marks[i] = atoi(pch);
    }
    
    s->marks[3] = atoi(pch+strlen(pch)+1);
    return 0;
}

void printStudent(struct student *s) {
    printInt(s->id);
    print("\t");
    char sec[2];
    sec[0] = s->section;
    sec[1] = '\0';
    print(sec);
    print("\t");
    float avg = 0;
    for(int i=0; i<4; ++i) {
        printInt(s->marks[i]);
        avg += (float)s->marks[i];
        print("\t");
    }
    print("Average : ");
    printFloat(avg/4);
    print("\n");
}

int read_line_from_file(int fd, char *buff, int buffsize) {
    memset(buff, 0, 100);
    int i=0;
    while(i<buffsize-1 && read (fd, &buff[i], 1) >= 0 && errno != EINTR) {
        if(buff[i] == '\n') {
            buff[i] = '\0';
            return 1;
        }
        i++;
        errno = 0;
    }
    return 0;
}

int main() {
    pid_t pid;
    pid = fork();

    if(pid < 0) {
        print("Error while forking!\n");
        exit(1);
    }
    else if(pid == 0) {
        //Child Process executes this
        int fd  = open("csv-os.csv", O_RDONLY);
        if(fd == -1) {
            fprintf(stderr, "Error opening file!\n");
            exit(0);
        }

        char str[100];

        read_line_from_file(fd, str, 100);
        
        while(read_line_from_file(fd, str, 100) > 0) {
            struct student new_student;
            parseStudent(str, &new_student);
            if(new_student.section == 'A')
                printStudent(&new_student);
        }

        close(fd);
        exit(0);
    }
    else {
        //Parent Process excutes this
        waitpid(-1, NULL, 0);
        print("Child Complete!\n");

        int fd  = open("csv-os.csv", O_RDONLY);
        if(fd == -1) {
            fprintf(stderr, "Error opening file!\n");
            return;
        }

        char str[100];

        read_line_from_file(fd, str, 100);
        
        while(read_line_from_file(fd, str, 100) > 0) {
            struct student new_student;
            parseStudent(str, &new_student);
            if(new_student.section == 'B')
                printStudent(&new_student);
        }

        close(fd);
    }
    return 0;
}