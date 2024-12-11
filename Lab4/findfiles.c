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
            //printf("test\n");
            indent++;
            recursive_directory(opendir(dp->d_name), counter, indent);
            indent--;
        }
        //printf("%d\n", counter);
            
        printf("%d", (int)fileStat.st_ino);
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
    // printf("Files in current directory:\n");
    // while ((dp = readdir(dir)) != NULL) {

    //     printf("%s\n", dp->d_name);
    // }
}


// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <dirent.h>
// #include <libgen.h>
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>

// void recursive_directory(const char *base_path, int counter, int indent) {
//     DIR *dir = opendir(base_path);
//     if (!dir) {
//         perror("opendir");
//         return;
//     }

//     struct dirent *dp;
//     struct stat fileStat;
//     char path[2];

//     while ((dp = readdir(dir)) != NULL) {
//         if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
//             // Construct full path
//             snprintf(path, sizeof(path), "%s/%s", base_path, dp->d_name);

//             // Get file stats
//             if (stat(path, &fileStat) == -1) {
//                 perror("stat");
//                 continue;
//             }

//             // Check if it's a directory
//             if (S_ISDIR(fileStat.st_mode)) {
//                 printf("Directory: %s (inode: %ld)\n", path, fileStat.st_ino);
//                 indent++;
//                 recursive_directory(path, counter, indent);
//                 indent--;
//             } else {
//                 // Print file info
//                 printf("%ld", (long)fileStat.st_ino);
//                 printf(" |");
//                 for (int i = 0; i < indent; i++) {
//                     printf(" - -");
//                 }
//                 printf(" > %s\n", dp->d_name);
//             }
//         }
//     }

//     closedir(dir);
// }

// int main() {
//     int counter = 0;
//     int indent = 0;
//     const char *base_path = "./";

//     recursive_directory(base_path, counter, indent);

//     return 0;
// }
