#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int compareAsc(const void *a, const void *b){
  return (*(int*)a - *(int*)b);
}

int compareDesc(const void *a, const void *b){
  return (*(int*)b - *(int*)a);
}

void fun(int n, int numThreads){
  int *A = (int*) malloc(n * sizeof(int));
  int *B = (int*) malloc(n * sizeof(int));

  for(int i=0; i<n; i++){
    A[i] = i+1;
    B[i] = n-i;
  }

  qsort(A, n, sizeof(int), compareAsc);
  qsort(B, n, sizeof(int), compareDesc);

  long long minScalarProd = 0;

  omp_set_num_threads(numThreads);

  double startT = omp_get_wtime();

  #pragma omp parallel for reduction(+:minScalarProd) 
  for (int i=0; i<n; i++) {
    minScalarProd += (A[i] * B[i]);
  }

  double endT = omp_get_wtime();

  printf("Compeleted Calculation with n:%d threads:%d in %f seconds\n", n, numThreads, endT - startT);
  printf("Minimum scaler product is : %lld \n", minScalarProd);

  return;
}

int main(int argc, char *argv[])
{
  fun(10000000, 1);
  fun(10000000, 4);
  fun(10000000, 8);
  fun(10000000, 12);
  return EXIT_SUCCESS;
}
