#include <stdio.h>

#include <omp.h>

#define N 10000000
#define NUM_THREADS 1

void main() {

    double avg=0.0, A[N];

    for(int i=0;i<N;i++) {
        A[i] = i+1;
    }

    #pragma omp parallel for reduction(+:avg) 
    for(int i=0;i<N;i++) {
        avg += A[i];
    }
    avg = avg/N;
}