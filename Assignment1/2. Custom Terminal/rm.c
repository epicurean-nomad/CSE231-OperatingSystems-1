#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

int check_dir(char *dirname) {
    // returns :-
    // 0 - not a valid directory
    // 2 - empty directory
    // 3 - non empty directory
    int n = 0;
    struct dirent *d;
    DIR *dir = opendir(dirname);
    if (dir == NULL){
        return 0;
    }
    while ((d = readdir(dir)) != NULL) {
        if(++n > 2)
            break;
    }
    closedir(dir);
    return n;
}

int remove_directory(const char *path) {
    DIR *d = opendir(path);
    int path_len = strlen(path);
    int err = -1;

    if (d) {
        struct dirent *p;
        err = 0;
        while (!err && (p=readdir(d))) {
            int err2 = -1;
            char newpath[200];
            strcpy(newpath, path);
            strcat(newpath, "/");
            strcat(newpath, p->d_name);

            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
                continue;

            if(p->d_type == DT_DIR){
                err2 = remove_directory(newpath);
            }
            else {
                err2 = remove(newpath);
            }
            err = err2;
        }
        closedir(d);
    }

    if (!err)
    err = rmdir(path);
    return err;
}

int main(int argc, char *argv[]) {
    // For Debugging Purpose only
    for(int i=0; i<argc; ++i) 
        printf("%s\t", argv[i]);
    printf("\n");

    // Modes of removing:
    // 0 - Non directories
    // 1 - Non directories + empty directories
    // 2 - everything
    int dirMode = 0;
    int arg = 1;

    while(arg < argc && argv[arg][0] == '-') {
        if(!strcmp(argv[arg], "-d") || !strcmp(argv[arg], "--dir")) {
            if(dirMode < 1)
                dirMode = 1;
        }

        else if(!strcmp(argv[arg], "-r") || !strcmp(argv[arg], "-R")
          || !strcmp(argv[arg], "--recursive")) {
            dirMode = 2;
        }

        else {
            fprintf(stderr, "rm: invalid option '%s'\n", argv[arg]);
            return 1;
        }
        arg++;
    }

    if(arg == argc) {
        printf("rm: missing operand\n");
        return 1;
    }

    while(arg < argc) {
        int type = check_dir(argv[arg]);
        int err = 0;
        
        if(type == 0) {
            err = remove(argv[argc]); 
        }
        else if(type == 2) {
            if(dirMode < 1) {
                fprintf(stderr, "rm: cannot remove '%s': Is a directory\n", argv[arg]);
            }
            else {
                err = remove_directory(argv[arg]);
            }
        }
        else if(type == 3) {
            if(dirMode < 1) {
                fprintf(stderr, "rm: cannot remove '%s': Is a directory\n", argv[arg]);
            }
            else if(dirMode < 2) {
                fprintf(stderr, "rm: cannot remove '%s': Directory not empty\n", argv[arg]);
            }
            else {
                err = remove_directory(argv[arg]);
            }
        }

        if(err) {
            err = errno;
            fprintf(stderr, "rm: cannot remove '%s': %s\n", argv[1], strerror(err));
        }
        arg++;
    }   

    return 0;
}