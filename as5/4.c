#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    int total_sum = 0; 

    #pragma omp parallel reduction(+:total_sum)
    {
        int partial_sum = 0; 

        #pragma omp for
        for (int i = 1; i <= 20; i++) {
            partial_sum += i;
            printf("%d by thread %d\n", partial_sum, omp_get_thread_num());
        }

        #pragma omp lastprivate(partial_sum)
        {
            total_sum = partial_sum; 
        }
    }

    printf("Total sum of the first 20 natural numbers = %d\n", total_sum);

    return EXIT_SUCCESS;
}

