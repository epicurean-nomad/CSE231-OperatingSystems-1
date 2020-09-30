#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <errno.h>

// Set this to the length of the executable file name 
// of the final program
#define filename_length 4

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

void pwd(char *args[]) {
    printf("%s\n", path);
}

void cd(char *args[]) {
    char newpath[100];
    if(args[1][0] == '/') {
        strcpy(newpath, args[1]);
    }
    else if(args[1][0] == '~' ) {
        strcpy(newpath, "/home/");
        strcat(newpath, getpwuid(getuid())->pw_name);
        strcat(newpath, args[1]+1);
    }
    else {
        strcpy(newpath, path);
        strcat(newpath, "/");
        strcat(newpath, args[1]);
    }

    //For debugging
    // printf("Go here : %s\n", newpath);

    chdir(newpath);
    getcwd(path, 100);
}

void history(char *args[]) {
    strcpy(temp, file_path);
    strcat(temp, "/history.bin");

    if(args[1] == NULL) {
        FILE *fptr;
        fptr = fopen(temp, "rb");

        int counter = 1;

        while(fscanf(fptr, "%255[^\n]%*c", temp) > 0) {
            printf("%5d  %s\n", counter++, temp);
        }

        fclose(fptr);
        return;
    }
    
    if(!strcmp(args[1], "-c")) {
        truncate(temp, 0);
    }
    else if(!strcmp(args[1], "-d")) {
        if(args[2] == NULL) {
            fprintf(stderr, "history: -d: option requirement an argument\n");
            return;
        }

        int offset = atoi(args[2]);

        FILE *fold, *fnew;
        fold = fopen(temp, "rb");
        char *newfile = malloc(strlen(temp+5));
        strcpy(newfile, temp);
        strcat(newfile, "t");
        fnew = fopen(newfile, "wb");

        int counter = 1;
        char buff[100];

        while(fscanf(fold, "%255[^\n]%*c", buff) > 0) {
            if(counter++ == offset)
                continue;
            fprintf(fnew, "%s\n", buff);
        }

        if(counter < offset) {
            fprintf(stderr, "history: %d: history position out of range\n", offset);
        }

        fclose(fold);
        fclose(fnew);
        remove(temp);
        rename(newfile, temp);
        free(newfile);
        return;

    }
    else {
        fprintf(stderr, "history: invalid option\n");
        return;
    }
}

void exit_(char *args[]) {
    exit(0);
}

void echo(char *args[]) {
    // Mode for trailing newline
    // 0 - Don't print
    // 1 - print
    int trailing_newline = 1;

    //Mode for backslash excapes
    // 0 - Disable
    // 1 - Enable
    int b_escapes = 0;

    int flag = 1;
    while(args[flag] != NULL && args[flag][0] == '-') {
        if(!strcmp(args[flag], "-n"))
            trailing_newline = 0;
        else if(!strcmp(args[flag], "-e"))
            b_escapes = 1;
        else if(!strcmp(args[flag], "-E"))
            b_escapes = 0;
        flag++;
    }

    while(args[flag] != NULL) {
        char *str = args[flag];

        for(int i=0; i<strlen(str); ++i) {
            char ch = str[i];
            if(ch == '\\' && b_escapes) {
                int done = 0;
                switch(str[i+1]) {
                    case 'n':
                        ch = '\n';
                        break;
                    case 't':
                        ch = '\t';
                        break;
                    case 'v':
                        ch = '\v';
                        break;
                    case 'b':
                        ch = '\b';
                        break;
                    case 'r':
                        ch = '\r';
                        break;
                    case 'f':
                        ch = '\f';
                        break;
                    case 'a':
                        ch = '\a';
                        break;
                    case '\\':
                        ch = '\\';
                        break;
                    case '?':
                        ch = '\?';
                        break;
                    case '\'':
                        ch = '\'';
                        break;
                    case '\"':
                        ch = '\"';
                        break;
                    default:
                        printf("\\%c", str[i+1]);
                        done = 1;
                }

                i++;
            }
            printf("%c", ch);
        }
        
        flag++;
    }

    if(trailing_newline)
        printf("\n");
}

//=======End of Inbuild Commands=======

//=============External Commands=======

void external_command(char *args[]) {
    pid_t id;
    id = fork();

    // For debugging purpose only
    // for(int i=0; i<2; ++i) 
    //     printf("%s\t", args[i]);
    // printf("\n");

    if(id < 0) {
        printf("Fork failed! Aborting...\n");
        return;
    }
    else if(id == 0) {
        strcpy(temp, file_path);
        strcat(temp, "/");
        strcat(temp, args[0]);
        
        // For debugging
        // printf("location: %s\n", temp);
        execvp(temp, args);

        // Only in case of error
        printf("Error while doing execv : %s\n", strerror(errno));
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

char** parse_args(char *str) {
    int len = strlen(str);
    if(len == 0)
        return NULL;

    //Count the number of arguments
    int count = 1;
    int special = 0;
    for(int i=0; i<len; ++i) {
        if(special) {
            if(str[i] == '"') {
                str[i] = '\0';
                special = 0;
            }

            continue;
        }

        if(str[i] == ' ') {
            str[i] = '\0';
            if(i+1<len && str[i+1]!=' ') {
                if(str[i+1] =='"') {
                    special = 1;
                    str[i] = '\0';
                    str[i+1] = '\0';
                }
                else if(str[i+1] == '+' && !strcmp(str, "date")) {
                    str[i] = '\0';
                    if(str[i+2] == 34){
                        str[i+1] = '\0';
                        str[i+2] = '+';

                        for(int j=i+2; j<len; ++j) {
                            if(str[j] == 34){
                                str[j] = '\0';
                                break;
                            }
                        }
                    }
                    count++;
                    break;
                }
                count++;
            }
        }
    }

    char **arr = malloc((count+1)*sizeof(char *));
    arr[0] = str;
    arr[count] = (char *) NULL;
    for(int i=1, k=1; i<len; ++i) {
        if(str[i] == '\0' && i+1<len && str[i+1] != '\0'){
            arr[k] = str+(i+1);
            // For debugging
            // printf("new: %s\n", arr[k]);
            k++;
        }
    }
    return arr;
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
    strcat(file_path, argv[0]+1);
    file_path[strlen(file_path)-filename_length-1] = '\0';

    while(1) {
        char line[100];
        memset(line, 0, 100);

        display_line();
        scanf("%255[^\n]%*c", line);
        add_history(line);

        char **args = parse_args(line);

        // For Debugging
        // printf("Args: %s\n", args);

        if(strcmp(args[0], "cd")==0) {
            cd(args);
        }
        else if(!strcmp(args[0], "pwd")) {
            pwd(args);
        }
        else if(!strcmp(args[0], "exit")) {
            exit_(args);
        }
        else if(!strcmp(args[0], "history")) {
            history(args);
        }
        else if(!strcmp(args[0], "echo")) {
            echo(args);
        }
        else if(!strcmp(args[0], "ls")) {
            external_command(args);
        }
        else if(!strcmp(args[0], "mkdir")) {
            external_command(args);
        }
        else if(!strcmp(args[0], "cat")) {
            external_command(args);
        }
        else if(!strcmp(args[0], "rm")) {
            external_command(args);
        }
        else if(!strcmp(args[0], "date")) {
            external_command(args);
        }

        free(args);
    }
}