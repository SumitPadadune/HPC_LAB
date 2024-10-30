#include <stdio.h>
#include <omp.h>

int main() {
    long long num_steps = 100000000; // Number of steps for the integration
    double step = 1.0 / (double)num_steps;
    double sum = 0.0;
    int i;

    double start_time = omp_get_wtime();
    #pragma omp parallel for reduction(+:sum)
    for (i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    double end_time = omp_get_wtime();

    double pi = step * sum;

    printf("Calculated value of Pi: %f\n", pi);
    printf("Execution Time: %f seconds\n", end_time - start_time);

    return 0;
}

