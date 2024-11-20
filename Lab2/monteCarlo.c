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

int hit;

void *func(int *hit) {

    pthread_detach(pthread_self());

    // if(t > 0) { // recursively spawn another thread

    //     pthread_t ptid;
    //     pthread_create(&ptid, NULL, &func, t-1);
    //     pthread_join(ptid, NULL);
        
    // }

    printf("Thread process!\n");
    *hit++;
    pthread_exit(NULL);

}

int main() {

    printf("Program start!\n");

    hit = 0;

    pthread_t *threads = (pthread_t*)malloc(THREADS*sizeof(pthread_t));

    for(int i = 0; i < THREADS; i++) // create THREADS amount of processes
        pthread_create(&threads[i], NULL, &func, &hit);

    for(int i = 0; i < THREADS; i++) // wait for all threads to exit
        pthread_join(threads[i], NULL);

    printf("Hit counter: %d\n", hit);

    // pthread_t ptid;
    // pthread_create(&ptid, NULL, &func, &hit);
    // pthread_join(ptid, NULL); // wait for thread to exit
    return 0;

}