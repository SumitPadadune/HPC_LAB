#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[])
{
  int numThreads;
  printf("Enter the number of threads : ");
  scanf("%d", &numThreads);

  // Sequential Output
  printf("\nSequential Output :\n");
  double seq_startT = omp_get_wtime();
  for(int i=0; i<numThreads; i++){
    printf("Hello World!!\n");
  }
  double seq_endT = omp_get_wtime();

  // Setting number of threads
  omp_set_num_threads(numThreads);

  // Prallel Output
  printf("\nPrallel Output\n");
  double par_startT = omp_get_wtime();
  #pragma omp parallel
  {
    printf("Hello World for thread : %d\n", omp_get_thread_num());
  }
  double par_endT = omp_get_wtime();
  
  printf("Time taked for sequtial code : %f seconds\n", seq_endT - seq_startT);
  printf("Time taken for parallel code : %f seconds\n", par_endT - par_startT);

  return 0;
}
