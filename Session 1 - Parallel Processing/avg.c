#include <stdio.h>

#include <omp.h>

#define N 100000
#define NUM_THREADS 1

void main() {

    double avg=0.0, A[N];

    for(int i=0;i<N;i++) {
        A[i] = i+1;
    }
    
    int i;

    #pragma omp parallel for reduction(+:avg) private(k) 
    for(i=0;i<N;i++) {
        avg += A[i];
    }
    avg = avg/N;
}
