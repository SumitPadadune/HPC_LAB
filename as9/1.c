#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 4  // Matrix size (NxN)

void matrix_vector_multiply(int rank, int size, int n, double* A, double* x, double* local_result) {
    int local_rows = n / size;  // Number of rows for each process
    for (int i = 0; i < local_rows; i++) {
        local_result[i] = 0;
        for (int j = 0; j < n; j++) {
            local_result[i] += A[i * n + j] * x[j];
        }
    }
}

// Sequential version of matrix-vector multiplication for single-process time
void sequential_matrix_vector_multiply(int n, double* A, double* x, double* result) {
    for (int i = 0; i < n; i++) {
        result[i] = 0;
        for (int j = 0; j < n; j++) {
            result[i] += A[i * n + j] * x[j];
        }
    }
}

int main(int argc, char** argv) {
    int rank, size;
    int n = N;  // Matrix size
    double *A = NULL, *x = NULL, *result = NULL;
    double *local_A, *local_result;
    int local_rows;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    local_rows = n / size;  // Number of rows per process

    // Allocate memory for local matrices and vectors
    local_A = (double*) malloc(local_rows * n * sizeof(double));
    local_result = (double*) malloc(local_rows * sizeof(double));

    // Master process initializes the matrix and vector
    if (rank == 0) {
        A = (double*) malloc(n * n * sizeof(double));
        x = (double*) malloc(n * sizeof(double));
        result = (double*) malloc(n * sizeof(double));

        // Initialize matrix A and vector x with random values
        srand(time(0));
        for (int i = 0; i < n * n; i++) {
            A[i] = rand() % 10;
        }
        for (int i = 0; i < n; i++) {
            x[i] = rand() % 10;
        }

        printf("Matrix A:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%6.2f ", A[i * n + j]);
            }
            printf("\n");
        }

        printf("\nVector x:\n");
        for (int i = 0; i < n; i++) {
            printf("%6.2f\n", x[i]);
        }
    }

    // Sequential execution to calculate T1
    double T1_start, T1_end, T1;
    if (rank == 0) {
        T1_start = MPI_Wtime();  // Start timing for sequential execution
        double* seq_result = (double*) malloc(n * sizeof(double));
        sequential_matrix_vector_multiply(n, A, x, seq_result);
        T1_end = MPI_Wtime();  // End timing for sequential execution
        T1 = T1_end - T1_start;

        printf("\nSequential Result vector:\n");
        for (int i = 0; i < n; i++) {
            printf("%6.2f\n", seq_result[i]);
        }
        free(seq_result);
    }

    // Scatter the matrix rows to all processes
    MPI_Scatter(A, local_rows * n, MPI_DOUBLE, local_A, local_rows * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Broadcast the vector to all processes
    if (rank == 0) {
        MPI_Bcast(x, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    } else {
        x = (double*) malloc(n * sizeof(double));
        MPI_Bcast(x, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    // Parallel execution to calculate Tp
    double Tp_start, Tp_end, Tp;
    Tp_start = MPI_Wtime();  // Start timing for parallel execution

    // Perform local matrix-vector multiplication
    matrix_vector_multiply(rank, size, n, local_A, x, local_result);

    // Gather the local results into the global result
    MPI_Gather(local_result, local_rows, MPI_DOUBLE, result, local_rows, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    Tp_end = MPI_Wtime();  // End timing for parallel execution
    Tp = Tp_end - Tp_start;

    // Master process prints the parallel result and calculates speedup
    if (rank == 0) {
        printf("\nParallel Result vector:\n");
        for (int i = 0; i < n; i++) {
            printf("%6.2f\n", result[i]);
        }

        // Calculate speedup: Speedup = T1 / Tp
        double speedup = T1 / Tp;
        printf("\nTime (Sequential - T1): %f seconds\n", T1);
        printf("Time (Parallel - Tp): %f seconds\n", Tp);
        printf("Speedup: %f\n", speedup);
    }

    // Clean up
    free(local_A);
    free(local_result);
    if (rank == 0) {
        free(A);
        free(x);
        free(result);
    } else {
        free(x);
    }

    MPI_Finalize();
    return 0;
}
