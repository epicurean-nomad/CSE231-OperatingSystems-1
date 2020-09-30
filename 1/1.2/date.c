#include <stdio.h>
#include <time.h>
#include <string.h>
#include <errno.h>

void display(char *format, struct tm *tmp) {
    char str[50];
    int err = strftime(str, sizeof(str), format, tmp);

    printf("%s\n", str);
}

void displayu(char *format, struct tm *tmp) {
    char str[50];
    char newformat[50];
    
    int err = strftime(str, sizeof(str), format, tmp);
    for(int i=0; i<strlen(str)-2; ++i) {
        if(str[i]=='G' && str[i+1]=='M' && str[i+2]=='T') {
            str[i] = 'U';
            str[i+1] = 'T';
            str[i+2] = 'C';
        }
    }
    printf("%s\n", str);
}

int main(int argc, char *argv[]) {
    time_t t ; 
    struct tm *tmp ;
    time( &t );
    tmp = localtime( &t ); 

    // For Debugging Purpose only
    // for(int i=0; i<argc; ++i) 
    //     printf("%s\t", argv[i]);
    // printf("\n");

    char *format = "%A %d %B %Y %r %Z";
    if(argc > 1) {
        if(argv[argc-1][0] == '+') {
            format = argv[argc-1] + 1;
            argc--;
        }
    }

    if(argc > 1) {
        if(!strcmp(argv[1], "-u")) {
            tmp = gmtime(&t);
            displayu(format, tmp);
            return 0;
        }
        else if(!strcmp(argv[1], "-R")) {
            format = "%a, %d %b %Y %H:%M:%S %z";
        }
        else {
            fprintf(stderr, "date: invalid option\n");
            return 1;
        }
        
    }
    display(format, tmp);
    
    return 0;
}