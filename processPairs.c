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

    int child_pid = fork();
    int my_pid = getpid();
    pid_t terminated_child = -1;

    if(child_pid == -1) // error
        printf("[%d] FORK ERROR\n", my_pid);

    if(child_pid == 0) { // child

        sleep(1);
        printf("[%d] Child process!\n", my_pid);
        const int bufsize = 256;
        char rbuf[bufsize];
        
        fd = open(name, O_RDONLY); // Open FIFO for reading
        read(fd,rbuf,bufsize); // Read from FIFO to buffer
        close(fd); // Close FIFO
        
        printf("[%d] Received: \"%s\"\n", my_pid, rbuf);
        exit(0); // terminates child

    } else { // parent

        printf("[%d] Parent process with child child_pid: %d\n", my_pid, child_pid);
        const char *wbuf = "This is the message sent in the queue";
        fd = open(name, O_WRONLY);
        write(fd, wbuf, strlen(wbuf) + 1);
        terminated_child = wait(&child_pid); // waits for child process to terminate before closing the queue
        close(fd);
        unlink(name);

    }

    if(terminated_child == -1)
        printf("[%d] No child was terminated\n", my_pid);
    else
        printf("[%d] Terminated child: %d\n", my_pid, terminated_child);
    
    printf("[%d] This process survived the terminations\n", my_pid);

    return 0;

}