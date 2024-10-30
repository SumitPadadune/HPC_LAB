#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MSG_SIZE 1024

int main(int argc, char *argv[]) {
    int rank, size;
    int message[MSG_SIZE];
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        for (int i = 0; i < MSG_SIZE; i++) {
            message[i] = i;
        }
        printf("Root process (Rank %d) broadcasting message...\n", rank);
    }

    start_time = MPI_Wtime();
    MPI_Bcast(message, MSG_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    printf("Process %d received message. First value: %d\n", rank, message[0]);

    if (rank == 0) {
        printf("Number of processes: %d, Time taken for broadcast: %f seconds\n", size, end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
