#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main() {

    const char *name = "/tmp/myfifo";
    int fd;

    mkfifo(name, 0666);

    int pid = fork();

    if(pid == -1) // error
        printf("FORK ERROR");

    if(pid == 0) { // child

        printf("Child process!");
        const int bufsize = 256;
        char rbuf[bufsize];
        
        fd = open(name, O_RDONLY); // Open FIFO for reading
        read(fd,rbuf,bufsize); // Read from FIFO to buffer
        close(fd); // Close FIFO
        
        printf("Received:\"%s\"\n",rbuf);

    } else { // parent

        printf("Parent process with child PID: %d", pid);
        const char *wbuf = "Hello";
        fd = open(name, O_WRONLY);
        write(fd, wbuf, strlen(wbuf) + 1);
        close(fd);
        unlink(name);

        printf("This is a process");

    }

    return 0;

}