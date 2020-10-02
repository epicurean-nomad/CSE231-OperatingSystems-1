#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

// Modes for displaying . and .. in results
// 0 - Do not display
// 1 - display
int back_dirs = 0;

// Modes for displaying entries starting with . (hidden entries)
// 0 - do not display
// 1 - display
int hidden = 0;

void set_blue() { printf("\033[1;34m"); }
void set_cyan() { printf("\033[1;36m"); }
void reset_color() { printf("\033[0m"); }

void print_dir(char *path) {
    DIR *d;
    struct dirent *dir;
    d = opendir(path);

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if(!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")) {
                if(!back_dirs)
                    continue;
            }
            else if(dir->d_name[0] == '.' && !hidden)
                continue;

            if(dir->d_type == DT_DIR)
                set_blue();

            if(dir->d_type == DT_LNK)
                set_cyan();

            printf("%s\n", dir->d_name);

            if(dir->d_type == DT_DIR || dir->d_type == DT_LNK)
                reset_color();
        }
        closedir(d);
    }
    else {
        fprintf(stderr, "ls: unbale to open directory '%s'\n", path);
    }
}

int main(int argc, char *argv[]) {
    int flag = 1;

    while(flag < argc && argv[flag][0] == '-') {
        if(!strcmp(argv[flag], "-a") || !strcmp(argv[flag], "--all")) {
            back_dirs = 1;
            hidden = 1;
        }
        else if(!strcmp(argv[flag], "-A") || !strcmp(argv[flag], "-almost-all")) {
            hidden = 1;
        }
        else {
            fprintf(stderr, "ls: invalid option '%s'\n", argv[flag]);
            return 1;
        }
        flag++;
    }

    

    // In case no argument is passed
    if(flag >= argc)
        print_dir(".");
    // In case when just one directory is given
    else if(argc == flag+1) {
        print_dir(argv[flag]);
    }
    else {
        while(flag < argc) {
            printf("%s:\n", argv[flag]);
            print_dir(argv[flag]);
            flag++;
            if(flag < argc)
                printf("\n");
        }
    }   
    
    return 0;
}