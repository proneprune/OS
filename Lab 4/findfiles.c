#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <libgen.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void recursive_directory(DIR *dir){
    //for each file in the directory
    //check if it is a directory
    //call recursive_directory on the directory
    //if it is a file, print the file name and inode number

}

int main() {
    DIR *dir;
    struct dirent *dp;

    dir = opendir("./");
    printf("Files in current directory:\n");
    while ((dp = readdir(dir)) != NULL) {

        printf("%s\n", dp->d_name);
    }
}