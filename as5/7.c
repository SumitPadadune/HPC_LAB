#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void parallel(int N) {
    int *array = (int *)malloc(N * sizeof(int));
    long long total_sum = 0;

    for (int i = 0; i < N; i++) {
        array[i] = 1;
    }

  clock_t start = clock();

  #pragma omp parallel for
  for (int i = 0; i < N; i++) {
    #pragma omp critical
    {
      total_sum += array[i];
    }
  }

  clock_t end = clock();

  double criticalTime = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Parallel Total Sum (with critical): %lld\n", total_sum);
  printf("Parallel Time (with critical): %f seconds\n", criticalTime);

  free(array);
  return;
}

void serial(int N) {
  int *array = (int *)malloc(N * sizeof(int));
  long long total_sum = 0;

  for (int i = 0; i < N; i++) {
    array[i] = 1; 
  }

  clock_t start = clock();
  for (int i = 0; i < N; i++) {
    total_sum += array[i];
  }
  clock_t end = clock();

  double serialTime = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Serial Total Sum: %lld\n", total_sum);
  printf("Serial Time: %f seconds\n", serialTime);

  free(array);
  return;
}

int main(int argc, char *argv[])
{ 
  serial(50000);
  parallel(50000);
  return EXIT_SUCCESS;
}

