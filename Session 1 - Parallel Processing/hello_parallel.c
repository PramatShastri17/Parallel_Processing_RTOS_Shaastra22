#include <stdio.h>

#include <omp.h>

void main() {
  #pragma omp parallel num_threads(4) 
  {
	int ID = omp_get_thread_num();
	printf(“Hello(%d)”, ID);
	printf(“ World(%d)\n”, ID);
  }
}
