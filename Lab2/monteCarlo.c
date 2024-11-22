#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <omp.h>
#include <pthread.h>
#include <math.h>

#pragma omp parallel

#define THREADS 4

const int msg_num_max = 10;
const int msg_size_max = 128;
const char *name ="/mymq";
int nHit = 0;
int nTry = 0;

int shoot() {

    unsigned short xsubiX[3] = {41,12,63};
    unsigned short xsubiY[3] = {125,312,124};

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

    // mqd_t mqd;
    // mqd = mq_open(name,O_WRONLY); //Open an EXISTING message queue

    // if(mqd == -1) {

    //     perror("mq_open");
    //     _exit(1);

    // }

    int shot = shoot();
    nHit += shot;
    nTry++;
    // mq_send(mqd,&shot,8,0);//Write messages to the queue

    printf("Thread process {\n");
    printf("\tnHit = %d;\n", nHit);
    printf("\tnTry = %d;\n", nTry);
    printf("};\n");
    pthread_exit(NULL);

}

int main() {

    printf("Program start!\n");

    int tHit = 0;

    for(int i = 0; i < 1000; i++)
        tHit += shoot();

    double tResult = tHit / 1000;

    printf("shoot test: %f\n", tResult);
    
    // char msg[msg_size_max];
    // mqd_t mqd;
    // struct mq_attr attr;
    // char buf[msg_size_max];
    // // Define the queue attributes
    // attr.mq_maxmsg = msg_num_max;
    // attr.mq_msgsize = msg_size_max;
    // // Create and open a message queue
    // mqd = mq_open(name, O_RDONLY | O_CREAT, 0666, &attr);

    // pthread_t *threads = (pthread_t*)malloc(THREADS*sizeof(pthread_t));
    pthread_t threads[THREADS];

    for(int i = 0; i < THREADS; i++) // create THREADS amount of processes
        pthread_create(&threads[i], NULL, &thread_function, NULL);

    for(int i = 0; i < THREADS; i++) // wait for all threads to exit
        pthread_join(threads[i], NULL);

    // int hit = 0;

    // for(int i = 0; i < THREADS; i++) {// add hits

    //     ssize_t ret = mq_receive(mqd, buf, msg_size_max, NULL);

    //     if (ret == -1) {

    //         perror("mq_receive");
    //         _exit(1);

    //     }

    //     hit += ret;
    // }

    printf("Hit counter: %d\n", nHit);
    free(threads);

    return 0;

}