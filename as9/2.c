#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 4  // Matrix size (NxN)

void matrix_multiply(int rank, int size, int n, double* A, double* B, double* local_C) {
    int local_rows = n / size;  // Number of rows assigned to each process

    for (int i = 0; i < local_rows; i++) {
        for (int j = 0; j < n; j++) {
            local_C[i * n + j] = 0;
            for (int k = 0; k < n; k++) {
                local_C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
}

// Sequential version for time comparison
void sequential_matrix_multiply(int n, double* A, double* B, double* C) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i * n + j] = 0;
            for (int k = 0; k < n; k++) {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
}

int main(int argc, char** argv) {
    int rank, size;
    int n = N;  // Matrix size
    double *A = NULL, *B = NULL, *C = NULL;
    double *local_A, *local_C;
    int local_rows;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    local_rows = n / size;  // Number of rows per process

    // Allocate memory for local matrices
    local_A = (double*) malloc(local_rows * n * sizeof(double));
    local_C = (double*) malloc(local_rows * n * sizeof(double));

    // Master process initializes matrices A and B
    if (rank == 0) {
        A = (double*) malloc(n * n * sizeof(double));
        B = (double*) malloc(n * n * sizeof(double));
        C = (double*) malloc(n * n * sizeof(double));

        // Initialize matrix A and B with random values
        srand(time(0));
        for (int i = 0; i < n * n; i++) {
            A[i] = rand() % 10;
            B[i] = rand() % 10;
        }

        printf("Matrix A:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%6.2f ", A[i * n + j]);
            }
            printf("\n");
        }

        printf("\nMatrix B:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%6.2f ", B[i * n + j]);
            }
            printf("\n");
        }
    }

    // Broadcast matrix B to all processes
    if (rank == 0) {
        MPI_Bcast(B, n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    } else {
        B = (double*) malloc(n * n * sizeof(double));
        MPI_Bcast(B, n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    // Scatter rows of matrix A to all processes
    MPI_Scatter(A, local_rows * n, MPI_DOUBLE, local_A, local_rows * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Perform local matrix multiplication
    double start_time = MPI_Wtime();  // Start parallel time
    matrix_multiply(rank, size, n, local_A, B, local_C);
    double end_time = MPI_Wtime();    // End parallel time

    // Gather the results back into matrix C in the master process
    MPI_Gather(local_C, local_rows * n, MPI_DOUBLE, C, local_rows * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Master process prints the final matrix C
    if (rank == 0) {
        printf("\nMatrix C (A * B):\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%6.2f ", C[i * n + j]);
            }
            printf("\n");
        }
        printf("\nParallel execution time: %f seconds\n", end_time - start_time);
    }

    // Sequential execution to calculate T1 for speedup comparison
    if (rank == 0) {
        double* seq_C = (double*) malloc(n * n * sizeof(double));
        double seq_start_time = MPI_Wtime();
        sequential_matrix_multiply(n, A, B, seq_C);
        double seq_end_time = MPI_Wtime();
        double sequential_time = seq_end_time - seq_start_time;

        printf("\nSequential execution time: %f seconds\n", sequential_time);

        // Speedup calculation
        double speedup = sequential_time / (end_time - start_time);
        printf("\nSpeedup: %f\n", speedup);

        free(seq_C);
    }

    // Clean up
    free(local_A);
    free(local_C);
    if (rank == 0) {
        free(A);
        free(B);
        free(C);
    } else {
        free(B);
    }

    MPI_Finalize();
    return 0;
}
