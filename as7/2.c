#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Group world_group;

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the rank of the process in MPI_COMM_WORLD
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get the group associated with MPI_COMM_WORLD
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);

    // Print rank and communicator group
    printf("Process %d: Rank = %d, Communicator Group = %p\n", rank, rank, (void*)world_group);

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}
