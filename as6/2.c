#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Function to multiply matrix A and vector v serially, store result in vector result
void serialMatrixVectorMultiply(int **A, int *v, int *result, int rowA, int colA) {
    for (int i = 0; i < rowA; i++) {
        result[i] = 0; // Initialize result[i] to 0
        for (int j = 0; j < colA; j++) {
            result[i] += A[i][j] * v[j];
        }
    }
}

// Function to multiply matrix A and vector v in parallel using OpenMP, store result in vector result
void parallelMatrixVectorMultiply(int **A, int *v, int *result, int rowA, int colA) {
    #pragma omp parallel for
    for (int i = 0; i < rowA; i++) {
        result[i] = 0; // Initialize result[i] to 0
        for (int j = 0; j < colA; j++) {
            result[i] += A[i][j] * v[j];
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

// Function to allocate a vector dynamically
int *allocateVector(int size) {
    return (int *)malloc(size * sizeof(int));
}

// Function to initialize a matrix with random integers
void initializeMatrix(int **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 10; // Random number between 0 and 9
        }
    }
}

// Function to initialize a vector with random integers
void initializeVector(int *vector, int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 10; // Random number between 0 and 9
    }
}

// Function to print a vector
void printVector(int *vector, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", vector[i]);
    }
    printf("\n");
}

// Main function
int main() {
    int rowA, colA;
    double start, end;

    // Input matrix dimensions
    printf("Enter rows and columns of matrix A: ");
    scanf("%d %d", &rowA, &colA);

    // Allocate and initialize matrix A and vector v
    int **A = allocateMatrix(rowA, colA);
    int *v = allocateVector(colA);
    int *result_serial = allocateVector(rowA); // For serial result
    int *result_parallel = allocateVector(rowA); // For parallel result

    // Initialize matrix A and vector v with random values
    initializeMatrix(A, rowA, colA);
    initializeVector(v, colA);

    // Perform serial matrix-vector multiplication
    start = omp_get_wtime();
    serialMatrixVectorMultiply(A, v, result_serial, rowA, colA);
    end = omp_get_wtime();
    printf("Serial Time: %f seconds\n", end - start);

    // Perform parallel matrix-vector multiplication using OpenMP
    start = omp_get_wtime();
    parallelMatrixVectorMultiply(A, v, result_parallel, rowA, colA);
    end = omp_get_wtime();
    printf("Parallel Time: %f seconds\n", end - start);

    // Compare the results (optional, for correctness check)
    int mismatch = 0;
    for (int i = 0; i < rowA; i++) {
        if (result_serial[i] != result_parallel[i]) {
            mismatch = 1;
            break;
        }
    }

    if (mismatch) {
        printf("Mismatch between serial and parallel results!\n");
    } else {
        printf("Results are identical.\n");
    }

    // Free dynamically allocated memory
    for (int i = 0; i < rowA; i++) free(A[i]);
    free(A);
    free(v);
    free(result_serial);
    free(result_parallel);

    return 0;
}

