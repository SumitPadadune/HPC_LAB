#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    int Aryabhatta = 10;

    #pragma omp parallel private(Aryabhatta)
    {
        Aryabhatta = 10; // setting value again cause it gives garbage value otherwise
        int thread_id = omp_get_thread_num();
        int result = thread_id * Aryabhatta;
        printf("Thread %d: result of thread ID * Aryabhatta = %d\n", thread_id, result);
    }

    return EXIT_SUCCESS;
}

