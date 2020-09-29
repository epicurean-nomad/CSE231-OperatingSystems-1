#include <dirent.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    DIR *d;
    struct dirent *dir;

    // In case no argument is passed
    if(argc <=1)
        d = opendir(".");
    // In case when a directory is given
    else
        d = opendir(argv[1]);
    
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
    return 0;
}