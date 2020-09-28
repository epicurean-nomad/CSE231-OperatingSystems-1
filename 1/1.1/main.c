#include <stdlib.h>
#include <stdio.h>
#include <syscall.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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

int parseInt(char *str) {
    int len = strlen(str);
    int ans = 0;
    for(int i=0; i<len; i++) {
        ans *= 10;
        ans += (int)(str[i]-48);
    }
    return ans;
}

int parseStudent(char *str, struct student *s) {
    char *pch;
    pch = strtok(str, ",");

    if(pch == NULL)
        return -1;
    
    s->id = parseInt(pch);
    pch = strtok(NULL, ",");

    if(pch == NULL)
        return -1;

    s->section = pch[0];

    for(int i=0; i<4; ++i) {
        pch = strtok(NULL, " ,");
        if(pch == NULL)
            return -1;
        s->marks[i] = parseInt(pch);
    }
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

int main() {
    pid_t pid;
    pid = fork();

    if(pid < 0) {
        print("Error while forking!\n");
        exit(1);
    }
    else if(pid == 0) {
        //Child Process executes this
        FILE *fptr  = fopen("csv-os.csv", "rb");
        char str[100];

        fscanf(fptr, "%255[^\n]%*c", str);
        
        while(fscanf(fptr, "%255[^\n]%*c", str) > 0) {
            struct student new_student;
            parseStudent(str, &new_student);
            if(new_student.section == 'A')
                printStudent(&new_student);
        }

        fclose(fptr);
        exit(0);
    }
    else {
        //Parent Process excutes this
        waitpid(-1, NULL, 0);
        print("Child Complete!\n");

        FILE *fptr  = fopen("csv-os.csv", "rb");
        char str[100];

        fscanf(fptr, "%255[^\n]%*c", str);
        
        while(fscanf(fptr, "%255[^\n]%*c", str) > 0) {
            struct student new_student;
            parseStudent(str, &new_student);
            if(new_student.section == 'B')
                printStudent(&new_student);
        }

        fclose(fptr);        
        exit(0);
    }
    return 0;
}