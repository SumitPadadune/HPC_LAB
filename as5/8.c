#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void parallel(int N) {
    int *array = (int *)malloc(N * sizeof(int));
    long long total_sum = 0;

    for (int i = 0; i < N; i++) {
        array[i] = 1; // Using 1 for simplicity
    }

    clock_t start = clock();
    #pragma omp parallel reduction(+:total_sum)
    {
        #pragma omp for
        for (int i = 0; i < N; i++) {
            total_sum += array[i];
        }
    }
    clock_t end = clock();

    double parallelTime = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Parallel Total Sum: %lld\n", total_sum);
    printf("Parallel Time: %f seconds\n", parallelTime);

    free(array);
    return;
}

void sequential(int N) {
    int *array = (int *)malloc(N * sizeof(int));
    long long total_sum = 0;

    for (int i = 0; i < N; i++) {
        array[i] = 1; // Using 1 for simplicity
    }

    clock_t start = clock();
    for (int i = 0; i < N; i++) {
        total_sum += array[i];
    }
    clock_t end = clock();

    double serialTime = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Sequential Total Sum: %lld\n", total_sum);
    printf("Sequential Time: %f seconds\n", serialTime);

    free(array);
    return;
}

int main(int argc, char *argv[])
{
  sequential(1000000);
  parallel(1000000);
  return EXIT_SUCCESS;
}

