#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <omp.h>
#include <pthread.h>

#pragma omp parallel

void *func(void *arg) {

    pthread_detach(pthread_self());
    printf("Thread process!\n");
    pthread_exit(NULL);

}

int main() {

    printf("Program start!\n");
    pthread_t ptid;
    pthread_create(&ptid, NULL, &func, NULL);
    return 0;

}