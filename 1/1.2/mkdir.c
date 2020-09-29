#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // For Debugging Purpose only
    // for(int i=0; i<argc; ++i) 
    //     printf("%s\t", argv[i]);
    // printf("\n");
    
    int flag=1;
    int mode = 777, verbose = 0;

    while(flag<argc && argv[flag][0]=='-') {
        if(!strcmp(argv[flag], "-m")) {
            mode = atoi(argv[flag++]);
        }
        else if(!strcmp(argv[flag], "-v")) {
            verbose = 1;
        }

        flag++;
    }

    //In case no argument is passed
    if(flag >= argc){
        printf("mkdir: missing operand\n");
        return 1;
    }

    for(int i=flag; i<argc; ++i) {
        int err = mkdir(argv[i], mode);
        if(err) {
            err = errno;
            fprintf(stderr, "mkdir: cannot create directory '%s': %s\n", argv[flag], strerror(err));
            continue;
        }
        if(verbose) {
            printf("mkdir: created directory '%s'\n", argv[i]);
        }
    }
    return 0;
}