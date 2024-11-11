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
        printf("\n[%d] FORK ERROR", my_pid);

    if(pid == 0) { // child

        printf("\n[%d] Child process!", my_pid);
        const int bufsize = 256;
        char rbuf[bufsize];
        
        fd = open(name, O_RDONLY); // Open FIFO for reading
        read(fd,rbuf,bufsize); // Read from FIFO to buffer
        close(fd); // Close FIFO
        
        printf("\n[%d] Received:\"%s\"\n", my_pid, rbuf);

    } else { // parent

        printf("\n[%d] Parent process with child PID: %d", my_pid, pid);
        const char *wbuf = "Hello";
        fd = open(name, O_WRONLY);
        write(fd, wbuf, strlen(wbuf) + 1);
        close(fd);
        unlink(name);

    }

    wait(500);
    kill(my_pid);

    return 0;

}