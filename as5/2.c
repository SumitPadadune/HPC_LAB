#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    int sum_of_squares = 0;

    #pragma omp parallel reduction(+:sum_of_squares)
    {
        int thread_id = omp_get_thread_num();
        int square = thread_id * thread_id;

        printf("Thread %d: square of thread ID = %d\n", thread_id, square);

        sum_of_squares += square;
    }

    printf("Sum of squares of thread IDs = %d\n", sum_of_squares);

    return EXIT_SUCCESS;
}

