#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define N 100
#define NTHRD 1

// For timekeeping
#include <sys/time.h>
#include <time.h>

long nano_seconds(struct timespec *t_start, struct timespec *t_stop)
{
    return (t_stop->tv_nsec - t_start->tv_nsec) +
           (t_stop->tv_sec - t_start->tv_sec) * 1000000000;
}

struct timespec t_start, t_stop;
long long measured_time;
// End of timekeeping

typedef struct { int imin; int imax; double psum; } args_t;

double *a;

void *partial_sum(void *p) {
    args_t *args = (args_t *) p;
    args->psum = 0.0;
    for (int i = args->imin; i < args->imax; i++)
        args->psum += a[i];
}

// int main() {
//     pthread_t thrd[NTHRD];
//     args_t thrd_args[NTHRD];

//     // Create input array
//     a = (double *) malloc(N * sizeof(double));
//     for (int i = 0; i < N; i++) 
//         a[i] = i;

//     clock_gettime(CLOCK_MONOTONIC, &t_start);
//     // Create threads
//     for (int i = 0; i < NTHRD; i++) {
//         thrd_args[i].imin = i * N /NTHRD;
//         thrd_args[i].imax = (i + 1) * N /NTHRD;
//         if (pthread_create(&thrd[i], NULL, partial_sum, &thrd_args[i])) {
//             perror("pthread_create");
//             _exit(1);
//         }
//     }

//     // Wait for threads to finish
//     for (int i = 0; i < NTHRD; i++)
//         if (pthread_join(thrd[i], NULL)) {
//             perror("pthread_join");
//             _exit(1);
//         }

//     // Compute global sum
//     double gsum = 0.0;
//     for(int i = 0; i < NTHRD; i++)
//         gsum += thrd_args[i].psum;

//     clock_gettime(CLOCK_MONOTONIC, &t_stop);
//     measured_time = nano_seconds(&t_start, &t_stop);

//     printf("gsum = %.1f\n", gsum);
//     printf("Threads: %d , Length: %d\n", NTHRD, N);
//     printf("Result obtained in %lld ns\n---------------------\n", measured_time);
// }

int main() {
    int length = N;

    for (int j = 0; j < 6; j++) {
        pthread_t thrd[NTHRD];
        args_t thrd_args[NTHRD];
        double gsum = 0.0;

        // Create input array
        a = (double *) malloc(length * sizeof(double));
        for (int i = 0; i < length; i++) 
            a[i] = i;

        clock_gettime(CLOCK_MONOTONIC, &t_start);

        for (int k = 0; k < 100; k++) {
            // Create threads
            for (int i = 0; i < NTHRD; i++) {
                thrd_args[i].imin = i * length /NTHRD;
                thrd_args[i].imax = (i + 1) * length /NTHRD;
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
            for(int i = 0; i < NTHRD; i++)
                gsum += thrd_args[i].psum;
            length = length*2;
        }

        clock_gettime(CLOCK_MONOTONIC, &t_stop);
        measured_time = nano_seconds(&t_start, &t_stop);

        printf("gsum = %.1f\n", gsum);
        printf("Threads: %d , Length: %d\n", NTHRD, length);
        printf("Result obtained in %lld ns\n---------------------\n", measured_time/100);
    }
}