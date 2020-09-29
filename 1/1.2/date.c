#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]) {
    time_t t ; 
    struct tm *tmp ; 
    char MY_TIME[50]; 
    time( &t );

    tmp = localtime( &t ); 
    strftime(MY_TIME, sizeof(MY_TIME), "%A %d %B %Y %r %Z", tmp); 

    printf("%s\n", MY_TIME);
}