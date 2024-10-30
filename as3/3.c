#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define VECTOR_SIZE 200
#define SCALAR 5.0

void vector_addition_static(int *vector, int size, int chunk_size) {
    #pragma omp parallel for schedule(static, chunk_size)
    for (int i = 0; i < size; i++) {
        vector[i] += SCALAR;
    }
}

void vector_addition_dynamic(int *vector, int size, int chunk_size) {
    #pragma omp parallel for schedule(dynamic, chunk_size)
    for (int i = 0; i < size; i++) {
        vector[i] += SCALAR;
    }
}

int main() {
    int i;
    int vector[VECTOR_SIZE];
    int chunk_sizes[] = {1, 10, 50, 100}; // Different chunk sizes for scheduling
    int num_chunk_sizes = sizeof(chunk_sizes) / sizeof(chunk_sizes[0]);
    
    // Initialize vector with values
    for (i = 0; i < VECTOR_SIZE; i++) {
        vector[i] = i;
    }

    // Static Scheduling
    printf("Static Scheduling:\n");
    for (i = 0; i < num_chunk_sizes; i++) {
        int chunk_size = chunk_sizes[i];
        double start_time = omp_get_wtime();
        vector_addition_static(vector, VECTOR_SIZE, chunk_size);
        double end_time = omp_get_wtime();
        printf("Chunk size %d: Time = %f seconds\n", chunk_size, end_time - start_time);
    }

    // Reset vector
    for (i = 0; i < VECTOR_SIZE; i++) {
        vector[i] = i;
    }

    // Dynamic Scheduling
    printf("Dynamic Scheduling:\n");
    for (i = 0; i < num_chunk_sizes; i++) {
        int chunk_size = chunk_sizes[i];
        double start_time = omp_get_wtime();
        vector_addition_dynamic(vector, VECTOR_SIZE, chunk_size);
        double end_time = omp_get_wtime();
        printf("Chunk size %d: Time = %f seconds\n", chunk_size, end_time - start_time);
    }

    // Demonstrate `nowait` clause
    int *vector1 = (int*)malloc(VECTOR_SIZE * sizeof(int));
    int *vector2 = (int*)malloc(VECTOR_SIZE * sizeof(int));

    // Initialize vectors
    for (i = 0; i < VECTOR_SIZE; i++) {
        vector1[i] = i;
        vector2[i] = i;
    }

    // Vector addition with `nowait` clause
    #pragma omp parallel
    {
        #pragma omp for nowait
        for (i = 0; i < VECTOR_SIZE; i++) {
            vector1[i] += SCALAR;
        }

        #pragma omp for
        for (i = 0; i < VECTOR_SIZE; i++) {
            vector2[i] += SCALAR;
        }
    }

    // Free allocated memory
    free(vector1);
    free(vector2);

    return 0;
}

