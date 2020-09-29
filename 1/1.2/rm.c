#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if(argc <= 1) {
        printf("rm: missing operand\n");
        return 1;
    }

    int err = remove(argv[1]);
    if(err) {
        err = errno;
        fprintf(stderr, "rm: cannot remove '%s': %s\n", argv[1], strerror(err));
        return 2;
    }
    return 0;
}