#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

#define THREADS 4
int nHit = 0;
int nTry = 0;
pthread_mutex_t mutex;

int shoot(int loop_index) {

    unsigned short xsubiX[3] = {pthread_self()+loop_index,pthread_self()+loop_index+2,pthread_self()+loop_index+3};
    unsigned short xsubiY[3] = {pthread_self()+loop_index+4,pthread_self()+loop_index+5,pthread_self()+loop_index+6};

    double randX = erand48(xsubiX); // [0,1)
    randX = (randX*2) - 1; // [-1,1)
    double randY = erand48(xsubiY); // [0,1)
    randY = (randY*2) - 1; // [-1,1)

    double dist = pow(randX,2)+pow(randY,2);

    if(dist > 1)
        return 0;
    return 1;
    
}

void *thread_function() {

    pthread_detach(pthread_self());
    int i = 0;

    while (1){
        pthread_mutex_lock(&mutex);
        int shot = shoot(i++);
        nHit += shot;
        nTry++;
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);

}

int main() {

    printf("Program start!\n");
    pthread_t threads[THREADS];

    for(int i = 0; i < THREADS; i++) // create THREADS amount of processes
        pthread_create(&threads[i], NULL, &thread_function, NULL);

    while(1){

        sleep(1);
        pthread_mutex_lock(&mutex);
        system("clear"); // check linux capability
        double ratio = 4 * (double)nHit/nTry;
        printf("UPDATE: nHit: %d, nTry: %d, ratio: %f\n", nHit, nTry, ratio);
        pthread_mutex_unlock(&mutex);

    }

    printf("Hit counter: %d\n", nHit);
    free(threads);

    return 0;

}