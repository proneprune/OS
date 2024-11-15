#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>

const int msg_num_max = 10;
const int msg_size_max = 128;
const char *name ="/mymq";


int main() {
    
    char msg[msg_size_max];
    mqd_t mqd;
    struct mq_attr attr;
    char buf[msg_size_max];
    // Define the queue attributes
    attr.mq_maxmsg = msg_num_max;
    attr.mq_msgsize = msg_size_max;
    // Create and open a message queue
    mqd = mq_open(name, O_RDONLY | O_CREAT, 0666, &attr);

    int status;
    int PID_L = fork();
    int PID_R = 2;

    // if(PID_L > 0)
        // PID_R = fork();

    if(PID_L == -1 || PID_R == -1)
        printf("PID = -1\n");

    else if(PID_L == 0 || PID_R == 0) { // child

        sleep(2);

        if (mqd == -1) {

            perror("mq_open");
            _exit(1);

        }

        // Read messages from the queue
        for (int i = 0; i <= msg_size_max; i++) { // TODO issue is prob here

            ssize_t ret = mq_receive(mqd, buf, msg_size_max, NULL);

            if (ret == -1) {

                perror("mq_receive");
                _exit(1);

            }

            printf("Received %ldBytes, message :\"%s\"\n", ret, buf);

        }

        // Close the message queue
        // mq_close(mqd);

    } else { // original parent

        const char *wmsg = "Hello! This is the parent transmitting";

        // mqd_t mqd;
        mqd = mq_open(name,O_WRONLY); //Open an EXISTING message queue

        if(mqd == -1) {

            perror("mq_open");
            _exit(1);

        }

        for(int i = 0; i < msg_size_max; i++) // TODO pls fix
            mq_send(mqd,&wmsg[i],i,0);//Write messages to the queue

        wait(&status);
        mq_close(mqd);//Close the message queue

    }

    return 0;

}