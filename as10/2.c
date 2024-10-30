#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int *message, *reduced_result = NULL;
    int message_size;
    double start_time, end_time;

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Loop over different message sizes
    for (message_size = 256; message_size <= 4096; message_size *= 2) {
        // Allocate memory for the message array
        message = (int*) malloc(message_size * sizeof(int));
        if (rank == 0) {
            // Only allocate memory for reduced_result on root process (rank 0)
            reduced_result = (int*) malloc(message_size * sizeof(int));
        }

        // Initialize the message array with values
        for (int i = 0; i < message_size; i++) {
            message[i] = rank + i;
        }

        // Synchronize processes and start timing the reduction operation
        MPI_Barrier(MPI_COMM_WORLD);
        start_time = MPI_Wtime();

        // Perform the MPI reduction (summing all elements)
        MPI_Reduce(message, reduced_result, message_size, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        // Stop timing
        end_time = MPI_Wtime();

        // Display the results and timing information on the root process
        if (rank == 0) {
            printf("Message size: %d, Number of processes: %d, Time taken for reduction: %f seconds\n", message_size, size, end_time - start_time);
            printf("Reduction result: First element = %d, Last element = %d\n", reduced_result[0], reduced_result[message_size - 1]);
        }

        // Free allocated memory
        free(message);
        if (rank == 0) {
            free(reduced_result);
        }
    }

    // Finalize the MPI environment
    MPI_Finalize();
    return 0;
}
