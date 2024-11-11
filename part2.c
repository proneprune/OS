#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define N 2048
#define NTHRD 4

// For timekeeping
#include <sys/time.h>
#include <time.h>

long nano_seconds(struct timespec *t_start, struct timespec *t_stop)
{
    return (t_stop->tv_nsec - t_start->tv_nsec) +
           (t_stop->tv_sec - t_start->tv_sec) * 1000000000;
}

struct timespec t_start, t_stop;
long long time;
// End of timekeeping

typedef struct { int imin; int imax; double psum; } args_t;

double *a;

void *partial_sum(void *p) {
    args_t *args = (args_t *) p;
    args->psum = 0.0;
    for (int i = args->imin; i < args->imax; i++)
        args->psum += a[i];
}

// // -------- ADD TO MAIN ----------
// clock_gettime(CLOCK_MONOTONIC, &t_start);
// clock_gettime(CLOCK_MONOTONIC, &t_stop);

// long long temp = nano_seconds(&t_start, &t_stop);

int main() {
    pthread_t thrd[NTHRD];
    args_t thrd_args[NTHRD];

    // Create input array
    a = (double *) malloc(N * sizeof(double));
    for (int i = 0; i < N; i++) 
        a[i] = i;

    // Create threads
    for (int i = 0; i < NTHRD; i++) {
        thrd_args[i].imin = i * N /NTHRD;
        thrd_args[i].imax = (i + 1) * N /NTHRD;
        if (pthread_create(&thrd[i], NULL, partial_sum, &thrd_args[i])) {
            perror("pthread_create");
            _exit(1);
        }
    }

    // Wait for threads to finish
    for (int i = 0; i < NTHRD; i++)
        if (pthread_join(thrd[i], NULL)) {
            perror("pthread_join");
            _exit(1);
        }

    // Compute global sum
    double gsum = 0.0;
    for(int i = 0; i < NTHRD; i++)
        gsum += thrd_args[i].psum;

    printf("gsum = %.1f\n", gsum);
}