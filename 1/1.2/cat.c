#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    //In case no argument is passed
    if(argc <= 1){
        printf("cat: missing operand\n");
        return 1;
    }

    FILE *fptr = NULL;
    fptr = fopen(argv[1], "rb");
    if(fptr == NULL) {
        printf("cat: %s: %s\n", argv[1], strerror(errno));
    }

    int c = getc(fptr);
    while (c != EOF) {
        putchar(c);
        c = getc(fptr);
    }
    printf("\n");
    fclose(fptr);
    return 0;
}