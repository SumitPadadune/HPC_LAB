#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    int send_data[100000];  // Large array to force blocking
    int recv_data[100000];

    // Initialize MPI environment
    MPI_Init(&argc, &argv);

    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Ensure we have at least 2 processes
    if (size < 2) {
        printf("This example requires at least two processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (rank == 0) {
        // Process 0 tries to send to Process 1, then receive from Process 1
        MPI_Send(send_data, 100000, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(recv_data, 100000, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else if (rank == 1) {
        // Process 1 tries to send to Process 0, then receive from Process 0
        MPI_Send(send_data, 100000, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(recv_data, 100000, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Output the results (this won't be reached due to deadlock)
    printf("Process %d completed communication.\n", rank);

    // Finalize MPI environment
    MPI_Finalize();

    return 0;
}
