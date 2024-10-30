#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    int send_data, recv_data;
    int left, right;

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Define the left and right neighbors in the ring
    left = (rank - 1 + size) % size;   // Left neighbor
    right = (rank + 1) % size;         // Right neighbor

    // Data to send (rank of the current process)
    send_data = rank;

    // Perform nearest neighbor exchange using blocking communication
    MPI_Send(&send_data, 1, MPI_INT, right, 0, MPI_COMM_WORLD);  // Send to the right neighbor
    MPI_Recv(&recv_data, 1, MPI_INT, left, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // Receive from the left neighbor

    // Output the result
    printf("Process %d received data %d from process %d\n", rank, recv_data, left);

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}
