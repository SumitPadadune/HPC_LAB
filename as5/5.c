#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void serialMatrixMultiplication(int **A, int **B, int **C, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void parallelMatrixMultiplicationStatic(int **A, int **B, int **C, int N) {
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void parallelMatrixMultiplicationDynamic(int **A, int **B, int **C, int N) {
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int N = 1000; // Matrix size
    int **A = (int **)malloc(N * sizeof(int *));
    int **B = (int **)malloc(N * sizeof(int *));
    int **C = (int **)malloc(N * sizeof(int *));
    
    for (int i = 0; i < N; i++) {
        A[i] = (int *)malloc(N * sizeof(int));
        B[i] = (int *)malloc(N * sizeof(int));
        C[i] = (int *)malloc(N * sizeof(int));
    }
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = 1;
            B[i][j] = 2;
        }
    }
    
    clock_t start, end;

    // Serial Execution
    start = clock();
    serialMatrixMultiplication(A, B, C, N);
    end = clock();
    double serialTime = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Serial Time: %f seconds\n", serialTime);

    // Static Scheduling
    start = clock();
    parallelMatrixMultiplicationStatic(A, B, C, N);
    end = clock();
    double staticTime = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Static Scheduling Time: %f seconds\n", staticTime);

    // Dynamic Scheduling
    start = clock();
    parallelMatrixMultiplicationDynamic(A, B, C, N);
    end = clock();
    double dynamicTime = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Dynamic Scheduling Time: %f seconds\n", dynamicTime);

    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);
    
    return 0;
}

