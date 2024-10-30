#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

void fun(int n, int threads){

  double scaler = 5.3;
  double *vec = (double*) malloc(n * sizeof(double));

  for(int i=0; i<n; i++){
    vec[i] = i % 38;
  }

  omp_set_num_threads(threads);

  double startT = omp_get_wtime();

  #pragma omp parallel for  
  for(int i=0; i<n; i++){
    vec[i] += scaler;
  }

  double endT = omp_get_wtime();

  printf("Time taked for vector scalar addition for n:%d threds:%d => %f seconds\n", n, threads, endT - startT);

  free(vec);
  return;
}

int main(int argc, char *argv[])
{
  fun(1000000, 6);
  fun(1000000, 8);
  fun(1000000, 10);
  fun(1000000, 20);
  return 0;
}
