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

int shoot() {

    unsigned short xsubiX[3] = {nTry,nTry*2,nTry*3};
    unsigned short xsubiY[3] = {nTry*3,nTry*4,nTry*5};

    double randX = erand48(xsubiX); // [0,1)
    randX = (randX*2) - 1; // [-1,1)
    double randY = erand48(xsubiY); // [0,1)
    randY = (randY*2) - 1; // [-1,1)

    double dist = sqrt(pow(randX,2)+pow(randY,2));

    if(dist > 1)
        return 0;
    return 1;
    
}

void *thread_function() {

    pthread_detach(pthread_self());

    while (1){
        int shot = shoot();
        nHit += shot;
        nTry++;
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
        printf("UPDATE: nHit: %f, nTry: %f\n", nHit, nTry);
        pthread_mutex_unlock(&mutex);

    }

    printf("Hit counter: %d\n", nHit);
    free(threads);

    return 0;

}