#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <omp.h>

#define N 100000000
#define NUM_THREADS 8

int A[N], B[N], C[N];

void main() {

    srand(time(0));

    for(int i=0;i<N;i++) {
        A[i] = rand();
        B[i] = rand();
    }

    double start = omp_get_wtime();

    omp_set_num_threads(NUM_THREADS);
    
    int k;

    #pragma omp parallel for private(k)
    for (k=0;k<N;k++) {
        C[k] = A[k] + B[k];
    }

    double end = omp_get_wtime();

    // Verification
    printf("Printing C[i] - A[i] - B[i]: Must evaluate to zero\n");
    for(int j=0;j<100;j++) {
        if((j%10==0) && j!=0) printf("\n");
        printf("%d ", (C[j]-A[j]-B[j]));
    }
    
    printf("\n");
    printf("Time taken: %lf seconds\n", (end-start));

}
