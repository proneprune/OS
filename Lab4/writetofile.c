
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>


#include <sys/time.h>
#include <time.h>

#define n_io 64
#define n_buf 128

// For timekeeping


double milli_seconds(struct timespec *t_start, struct timespec *t_stop) {
    return (t_stop->tv_sec - t_start->tv_sec) * 1000.0 + 
           (t_stop->tv_nsec - t_start->tv_nsec) / 1e6;
}

struct timespec t_start, t_stop;
double measured_time;
// End of timekeeping

// default variable = FFFFFFFF
// skapa fil
// loop körs 2^30 / 8 gånger
// i loopen: fputs(dv)
// ta bort fil


int main(){
    char dv[n_io] = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF\n\0";
    char buf[n_buf];
    FILE *fptr;
    fptr = fopen("file.txt", "w");
    if(fptr == NULL){
        printf("Error!");
        exit(1);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_start);

    //fprintf(fptr, "Hello World!");
    for(int i = 0; i < (pow(2, 30)/n_io); i++){
        //fputs(dv, fptr);
        setvbuf(fptr, buf, _IOFBF, sizeof(buf));

    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    measured_time = milli_seconds(&t_start, &t_stop);

    printf("Result obtained in %f ms\n---------------------\n", measured_time);


    }
    
    
    
    
    fclose(fptr);
    return 0;
}



// int main(){
//     char dv[n_io] = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF\n\0";
//     FILE *fptr;
//     fptr = fopen("file.txt", "w");
//     if(fptr == NULL){
//         printf("Error!");
//         exit(1);
//     }
//     //fprintf(fptr, "Hello World!");
//     for(int i = 0; i < (pow(2, 30)/n_io); i++){
//         fputs(dv, fptr);
//     }
    
    
    
    
//     fclose(fptr);
//     return 0;
// }