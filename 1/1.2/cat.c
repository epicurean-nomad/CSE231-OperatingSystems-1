#include <stdio.h>
#include <errno.h>
#include <string.h>

// Show ends - Print $ at the end of the line
// 0 - Don't show $
// 1 - Show $
int show_ends = 0;

// Number all output lines
// 0 - Don't number lines
// 1 - number lines
int number_lines = 0;

int read_file_line(char *path) {
    FILE *fptr = NULL;
    fptr = fopen(path, "rb");
    if(fptr == NULL) {
        fprintf(stderr, "cat: %s: %s\n", path, strerror(errno));
        return 1;
    }

    int c = getc(fptr);
    int line_no = 1;
    while (c != EOF) {
        // Checking if we encounter a new line
        if(c == 10 && show_ends)
            printf("$");
        
        putchar(c);

        if(c==10 && number_lines)
            printf("%5d  ", line_no++);

        c = getc(fptr);
    }
    
    fclose(fptr);
    return 0;
}

int main(int argc, char *argv[]) {
    int flag = 1, err = 0;

    while(flag < argc && argv[flag][0]=='-') {
        if(!strcmp(argv[flag], "-n") || !strcmp(argv[flag], "--number"))
            number_lines = 1;
        else if(!strcmp(argv[flag], "-E") || !strcmp(argv[flag], "--show-ends"))
            show_ends = 1;
        else {
            fprintf(stderr, "cat: invalid option -- '%s'\n", argv[flag]);
            return 2;
        }
        flag++;
    }

    //In case no argument is passed
    if(flag == argc){
        fprintf(stderr, "cat: missing operand\n");
        return 1;
    }
    
    
    while(flag < argc) {
        int t = read_file_line(argv[flag]);

        if(t > err)
            err = t;

        if(flag+1 != argc)
            printf("\n");
        
        flag++;
    }    

    return err;
}