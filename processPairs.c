#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main() {

    const char *name = "/tmp/myfifo";
    const char *wbuf = "Hello";
    int fd;

    mkfifo(name, 0666);

    fd = open(name, O_WRONLY);
    write(fd, wbuf, strlen(wbuf) + 1);
    close(fd);
    unlink(name);

    printf("This is a process");
    fork();
    return 0;

}