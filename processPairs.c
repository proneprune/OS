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

    int my_pid = getpid();
    int pid = fork();

    if(pid == -1) // error
        printf("[%d] FORK ERROR\n", my_pid);

    if(pid == 0) { // child

        printf("[%d] Child process!\n", my_pid);
        const int bufsize = 256;
        char rbuf[bufsize];
        
        fd = open(name, O_RDONLY); // Open FIFO for reading
        read(fd,rbuf,bufsize); // Read from FIFO to buffer
        close(fd); // Close FIFO
        
        printf("[%d] Received:\"%s\"\n", my_pid, rbuf);

    } else { // parent

        printf("[%d] Parent process with child PID: %d\n", my_pid, pid);
        const char *wbuf = "Hello";
        fd = open(name, O_WRONLY);
        write(fd, wbuf, strlen(wbuf) + 1);
        close(fd);
        unlink(name);

    }

    wait(500);
    kill(my_pid);
    printf("This should not be printed, because the process is terminated\n");

    return 0;

}