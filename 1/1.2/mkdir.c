#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    //In case no argument is passed
    if(argc <= 1){
        printf("mkdir: missing operand\n");
        return 1;
    }
    int err = mkdir(argv[1], 0777);
    if(err) {
        err = errno;
        fprintf(stderr, "mkdir: cannot create directory '%s': %s\n", argv[1], strerror(err));
        return 2;
    }
    return 0;
}