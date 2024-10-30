#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int rank, size, n = 10;
    int A[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};  // Example array of size 10
    int local_sum = 0, global_sum = 0;
    int i, start, end;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) {
            printf("This program requires exactly 2 processes.\n");
        }
        MPI_Finalize();
        return -1;
    }

    if (rank == 0) {
        start = 0;
        end = n / 2;
    } else if (rank == 1) {
        start = n / 2;
        end = n;
    }

    for (i = start; i < end; i++) {
        local_sum += A[i];
    }
    printf("Process %d local sum: %d\n", rank, local_sum);

    if (rank == 0) {
        int received_sum;
        MPI_Recv(&received_sum, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        global_sum = local_sum + received_sum;
        printf("Final sum: %d\n", global_sum);  // P0 prints the final result
    } else if (rank == 1) {
        MPI_Send(&local_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();

    return 0;
}
