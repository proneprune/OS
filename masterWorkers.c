#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct child_stack {

    int *PIDs;
    int length;
    int SP;

} child_stack;

typedef struct msg_node {

    struct msg_node *next;
    char *msg;
    int receiver;

} msg_node;

typedef struct msg_queue {

    msg_node *first;
    msg_node *last;

} msg_queue;

child_stack *create_child_stack(int length) {

    child_stack *new = (child_stack*)malloc(sizeof(child_stack));
    new->PIDs = (int*)malloc(length*sizeof(int));
    new->length = length;
    new->SP = 0;

    for(int i = 0; i < length; i++)
        new->PIDs[i] = -1;

    return new;

}

child_stack *extend_child_stack(child_stack *cs, int new_length) {

    child_stack *new = create_child_stack(new_length);
    new->length = new_length;
    new->SP = cs->SP;

    if(cs->SP > new_length)
        new->SP = new_length;

    for(int i = 0; i < new_length && i < cs->length; i++)
        new->PIDs[i] = cs->PIDs[i];

    return new;

}

child_stack *add_to_child_stack(child_stack *cs, int PID) {

    if(cs == NULL)
        cs = create_child_stack(4);

    if(cs->SP == cs->length)
        cs = extend_child_stack(cs, cs->length + 4);

    cs->PIDs[cs->SP++] = PID;

    return cs;

}

msg_node *create_msg_node() {

    msg_node *new = (msg_node*)malloc(sizeof(msg_node));
    new->msg = "";
    new->next = NULL;
    new->receiver = -1;

    return new;

}

msg_queue *create_msg_queue() {

    msg_queue *new = (msg_queue*)malloc(sizeof(msg_queue));
    new->first = NULL;
    new->last = NULL;

    return new;

}

msg_node *dequeue_msg(msg_queue *q) {

    msg_node *t = q->first;

    if(t != NULL)
        q->first = t->next;

    return t;
    
}

msg_queue *enqueue_msg(msg_queue *q, msg_node *m) {

    msg_node *last = q->last;

    if(last == NULL)
        q->first = m;
    
    else {

        q->last = m;
        last->next = m;

    }

    return q;
        
}

// TODO message queue to send/receive to every family
// int as intended receiver and switch case on negative
// to specify all/parent/error

int main() {

    printf("PROGRAM START\n");

    int iterations = 4;
    child_stack *cs = NULL;
    int child_pid = -1;
    int my_pid = getpid();

    for(int i = 0; i < iterations; i++) {

        child_pid = fork();
        my_pid = getpid();

        if(child_pid == -1) // error
            printf("[%d] FORK ERROR\n", my_pid);

        if(child_pid == 0) { // child

            printf("[%d] Child process.\n", my_pid);
            cs = create_child_stack(4);

        } else { // parent

      	    printf("[%d] Parent process with child child_pid: %d\n", my_pid, child_pid);
            cs = add_to_child_stack(cs, child_pid);

	    }

    }

    printf("[%d] Child count: %d\n", my_pid, cs->SP);
    printf("[%d] PROCESS END\n", my_pid);
    return 0;

}
