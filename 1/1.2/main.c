#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>

//==========Global Variables=============

//For Storing Path to Current Working Directory
char path[100];
char file_path[100];

//For storing the username and hostname
char *user, host[50];

//For storing temporary buffer
char temp[100];

//======================================

//=======Start of Inbuild commands======

void pwd(char *args) {
    printf("%s\n", path);
}

void cd(char *args) {
    char newpath[100];
    if(args[0] != '/') {
        strcpy(newpath, path);
        strcat(newpath, "/");
        strcat(newpath, args);
    }
    else {
        strcpy(newpath, args);
    }

    //For debugging
    // printf("Go here : %s\n", newpath);

    chdir(newpath);
    getcwd(path, 100);
}

void history(char *args) {
    FILE *fptr;
    strcpy(temp, file_path);
    strcat(temp, "/history.bin");
    fptr = fopen(temp, "rb");

    int counter = 1;

    while(fscanf(fptr, "%255[^\n]%*c", temp) > 0) {
        printf("%5d  %s\n", counter++, temp);
    }

    fclose(fptr);
}

void exit_(char *args) {
    exit(0);
}

void echo(char *args) {
    printf("%s\n", args);
}

//=======End of Inbuild Commands=======

//=============External Commands=======

void ls(char *args) {
    pid_t id;
    id = fork();

    if(id < 0) {
        printf("Fork failed! Aborting...\n");
        return;
    }
    else if(id == 0) {
        strcpy(temp, file_path);
        strcat(temp, "/ls");
        
        if(!strcmp(args, ""))
            execl(temp, "ls", (char *)NULL);
        else
            execl(temp, "ls", args, (char *)NULL);

        // Only in case of error
        exit(0);
    }
    else {
        wait(NULL);
        return;
    }
}

void date(char *args) {
    pid_t id;
    id = fork();

    if(id < 0) {
        printf("Fork failed! Aborting...\n");
        return;
    }
    else if(id == 0) {
        strcpy(temp, file_path);
        strcat(temp, "/date");
        
        if(!strcmp(args, ""))
            execl(temp, "date", (char *)NULL);
        else
            execl(temp, "date", args, (char *)NULL);

        // Only in case of error
        exit(0);
    }
    else {
        wait(NULL);
        return;
    }
}

void cat(char *args) {
    pid_t id;
    id = fork();

    if(id < 0) {
        printf("Fork failed! Aborting...\n");
        return;
    }
    else if(id == 0) {
        strcpy(temp, file_path);
        strcat(temp, "/cat");
        
        if(!strcmp(args, ""))
            execl(temp, "cat", (char *)NULL);
        else
            execl(temp, "cat", args, (char *)NULL);

        // Only in case of error
        exit(0);
    }
    else {
        wait(NULL);
        return;
    }
}

void rm(char *args) {
    pid_t id;
    id = fork();

    if(id < 0) {
        printf("Fork failed! Aborting...\n");
        return;
    }
    else if(id == 0) {
        strcpy(temp, file_path);
        strcat(temp, "/rm");
        
        if(!strcmp(args, ""))
            execl(temp, "rm", (char *)NULL);
        else
            execl(temp, "rm", args, (char *)NULL);

        // Only in case of error
        exit(0);
    }
    else {
        wait(NULL);
        return;
    }
}

void mkdir(char *args) {
    pid_t id;
    id = fork();

    if(id < 0) {
        printf("Fork failed! Aborting...\n");
        return;
    }
    else if(id == 0) {
        strcpy(temp, file_path);
        strcat(temp, "/mkdir");
        
        if(!strcmp(args, ""))
            execl(temp, "mkdir", (char *)NULL);
        else
            execl(temp, "mkdir", args, (char *)NULL);

        // Only in case of error
        exit(0);
    }
    else {
        wait(NULL);
        return;
    }
}

//=======End of External Commands======

//=======Start of helper functions=====

void set_yellow() { printf("\033[01;33m"); }

void set_cyan() { printf("\033[1;36m"); }

void reset_color() { printf("\033[0m"); }

void display_line() {
    set_yellow();
    printf("%s@%s", user, host);
    reset_color();
    printf(":");

    set_cyan();
    printf("%s", path);
    reset_color();
    printf("$ ");
}

void add_history(char *args) {
    FILE *h_file;
    strcpy(temp, file_path);
    strcat(temp, "/history.bin");
    h_file = fopen(temp, "ab");
    fprintf(h_file, "%s\n", args);
    fclose(h_file);
}

void setup() {
    getcwd(path, 100);
    strcpy(file_path, path);
    user = getpwuid(getuid())->pw_name;
    
    // For Debugging Purpose
    // printf("User: %s\n", user);

    gethostname(host, 50);
}
//========End of helper functions======


int main(int argc, char *argv[]) {
    setup();

    while(1) {
        char line[100];
        memset(line, 0, 100);

        display_line();
        scanf("%255[^\n]%*c", line);
        add_history(line);
        strcpy(temp, line);

        char *ptr = strtok(temp, " ");
        int offset = strlen(ptr);
        char *args = line+offset+1;

        // For Debugging
        // printf("Args: %s\n", args);

        if(strcmp(ptr, "cd")==0) {
            cd(args);
        }
        else if(!strcmp(ptr, "pwd")) {
            pwd(args);
        }
        else if(!strcmp(ptr, "exit")) {
            exit_(args);
        }
        else if(!strcmp(ptr, "history")) {
            history(args);
        }
        else if(!strcmp(ptr, "echo")) {
            echo(args);
        }
        else if(!strcmp(ptr, "ls")) {
            ls(args);
        }
        else if(!strcmp(ptr, "mkdir")) {
            mkdir(args);
        }
        else if(!strcmp(ptr, "cat")) {
            cat(args);
        }
        else if(!strcmp(ptr, "rm")) {
            rm(args);
        }
        else if(!strcmp(ptr, "date")) {
            date(args);
        }
    }
}