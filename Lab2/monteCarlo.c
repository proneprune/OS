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

#define THREADS 4

void *func(int t) {

    pthread_detach(pthread_self());
    printf("Thread process!%d\n", t);
    pthread_exit(NULL);

}

int main() {

    int t = THREADS;

    printf("Program start!\n");
    pthread_t ptid;
    pthread_create(&ptid, NULL, &func, &t);
    pthread_join(ptid, NULL); // wait for thread to exit
    return 0;

}