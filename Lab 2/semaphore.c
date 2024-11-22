#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdatomic.h>

// ---------- TIMEKEEPING ----------
#include <sys/time.h>
#include <time.h>

double seconds(struct timespec* t_start, struct timespec* t_stop) {
    return (t_stop->tv_sec - t_start->tv_sec) + 
           (t_stop->tv_nsec - t_start->tv_nsec) / 1e9;
}

struct timespec t_start, t_stop;
double measured_time;
// ---------- END OF TIMEKEEPING ----------

int counter = 0;
const int nthr = 1000;

typedef struct {
    /* MEMBERS TO BE DEFINED */
    volatile atomic_flag flg;
    // bool is_free;
} mysemaphore_t;

mysemaphore_t sem;

int sem_init(mysemaphore_t* s) {
    /* FUNCTION TO BE IMPLEMENTED */
    atomic_flag_clear(&s->flg);
    return 0;
}

int sem_wait(mysemaphore_t* s) {
    /* FUNCTION TO BE IMPLEMENTED */
    int delay = 100;
    while (atomic_flag_test_and_set(&s->flg)) {
        // usleep(delay);
        // delay *= 2;  
        // if (delay > 1000) delay = 1000; // Limit delay to 1 millisec
    }
    return 0;
}

int sem_post(mysemaphore_t* s) {
    /* FUNCTION TO BE IMPLEMENTED */
    atomic_flag_clear(&s->flg);
    return 0;
}

void* func() {
    sleep(1);
    // sem_wait(&sem);
    counter++;
    sem_post(&sem);
    return NULL;
}

int main() {
    clock_gettime(CLOCK_MONOTONIC, &t_start);

    pthread_t thr[nthr];

    sem_init(&sem);

    for (int i = 0; i < nthr; i++) {
        pthread_create(&thr[i], NULL, func, NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    measured_time = seconds(&t_start, &t_stop);

    for (int i = 0; i < nthr; i++) {
        pthread_join(thr[i], NULL);
    }

    printf("Part 2 time : %d\n", measured_time);
    printf("counter = %d\n", counter);

    return 0;
}
