#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Function to multiply two matrices A and B serially, store result in C
void serialMatrixMultiply(int **A, int **B, int **C, int rowA, int colA, int colB) {
    for (int i = 0; i < rowA; i++) {
        for (int j = 0; j < colB; j++) {
            C[i][j] = 0; // Initialize C[i][j]
            for (int k = 0; k < colA; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Function to multiply two matrices A and B in parallel using OpenMP, store result in C
void parallelMatrixMultiply(int **A, int **B, int **C, int rowA, int colA, int colB) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < rowA; i++) {
        for (int j = 0; j < colB; j++) {
            C[i][j] = 0; // Initialize C[i][j]
            for (int k = 0; k < colA; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Function to allocate a matrix dynamically
int **allocateMatrix(int rows, int cols) {
    int **matrix = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int *)malloc(cols * sizeof(int));
    }
    return matrix;
}

// Function to initialize a matrix with random integers
void initializeMatrix(int **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 10; // Random number between 0 and 9
        }
    }
}

// Function to print a matrix
void printMatrix(int **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Main function
int main() {
    int rowA, colA, rowB, colB;
    double start, end;

    // Input matrix dimensions
    printf("Enter rows and columns of matrix A: ");
    scanf("%d %d", &rowA, &colA);

    printf("Enter rows and columns of matrix B: ");
    scanf("%d %d", &rowB, &colB);

    if (colA != rowB) {
        printf("Matrix multiplication not possible with given dimensions!\n");
        return -1;
    }

    // Allocate and initialize matrices
    int **A = allocateMatrix(rowA, colA);
    int **B = allocateMatrix(rowB, colB);
    int **C_serial = allocateMatrix(rowA, colB); // For serial result
    int **C_parallel = allocateMatrix(rowA, colB); // For parallel result

    // Initialize A and B with random values
    initializeMatrix(A, rowA, colA);
    initializeMatrix(B, rowB, colB);

    // Perform serial matrix multiplication
    start = omp_get_wtime();
    serialMatrixMultiply(A, B, C_serial, rowA, colA, colB);
    end = omp_get_wtime();
    printf("Serial Time: %f seconds\n", end - start);

    // Perform parallel matrix multiplication using OpenMP
    start = omp_get_wtime();
    parallelMatrixMultiply(A, B, C_parallel, rowA, colA, colB);
    end = omp_get_wtime();
    printf("Parallel Time: %f seconds\n", end - start);

    // Compare the matrices (optional, for correctness check)
    int mismatch = 0;
    for (int i = 0; i < rowA; i++) {
        for (int j = 0; j < colB; j++) {
            if (C_serial[i][j] != C_parallel[i][j]) {
                mismatch = 1;
                break;
            }
        }
        if (mismatch) break;
    }

    if (mismatch) {
        printf("Mismatch between serial and parallel results!\n");
    } else {
        printf("Results are identical.\n");
    }

    // Free dynamically allocated memory
    for (int i = 0; i < rowA; i++) free(A[i]);
    for (int i = 0; i < rowB; i++) free(B[i]);
    for (int i = 0; i < rowA; i++) free(C_serial[i]);
    for (int i = 0; i < rowA; i++) free(C_parallel[i]);

    free(A);
    free(B);
    free(C_serial);
    free(C_parallel);

    return 0;
}

