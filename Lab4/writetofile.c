
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>


#include <sys/time.h>
#include <time.h>

#define n_io 64
#define n_buf 1024

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
    double total_time = 0.0;
    for(int j = 0; j<10; j++){
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

    


    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    measured_time = milli_seconds(&t_start, &t_stop);
    total_time += measured_time;
    fclose(fptr);
    }

    printf("Result obtained in %f ms\n---------------------\n", total_time/10);
    
    
    
    
    
    return 0;
}



 /*int main(){
     char dv[n_io] = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF\n\0";
    double total_time = 0.0;

     
     for(int j = 0; j < 10; j++){
     FILE *fptr;
     
     fptr = fopen("file.txt", "w");
     if(fptr == NULL){
         printf("Error!");
         exit(1);
     }
    
     clock_gettime(CLOCK_MONOTONIC, &t_start);
     //fprintf(fptr, "Hello World!");
     for(int i = 0; i < (pow(2, 30)/n_io); i++){
         fputs(dv, fptr);
     }
    
    
    
    
     fclose(fptr);

    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    measured_time = milli_seconds(&t_start, &t_stop);
    total_time += measured_time;
    }

    printf("Result obtained in %f ms\n---------------------\n", total_time/10);

     return 0;
 }*/