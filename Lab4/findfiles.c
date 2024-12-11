#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <libgen.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void recursive_directory(DIR *dir, int counter, int indent){
    //for each file in the directory
    //check if it is a directory
    //call recursive_directory on the directory
    //if it is a file, print the file name and inode number
    
    struct dirent *dp;
    struct stat fileStat;
    while ((dp = readdir(dir)) != NULL) {
        if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0){
        counter++;
        // check if directory or file
        stat(dp->d_name, &fileStat);
        if(counter == 20){
            break;
        }

        if( S_ISDIR(fileStat.st_mode) && strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0){
            indent++;
            recursive_directory(opendir(dp->d_name), counter, indent);
            indent--;
        }
            
        printf("%8d", (int)fileStat.st_ino);
        printf(" |");
        for(int i = 0; i < indent; i++){
            printf(" - -");
        }
        printf(" > %s\n ", dp->d_name);
        }
    }
}

int main() {
    DIR *dir;
    dir = opendir(".");
    int counter = 0;
    int indent = 0;

    recursive_directory(dir, counter, indent);

}
