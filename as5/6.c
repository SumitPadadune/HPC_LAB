#include <stdio.h>
#include <omp.h>

void printSeries(int startValue, int increment, int count) {
    for (int i = 0; i < count; i++) {
        printf("%d(%d) ", startValue + i * increment, startValue);
    }
    printf("\n \n");
}

int main() {
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            printSeries(2, 2, 20); 
        }
        
        #pragma omp section
        {
            printSeries(4, 4, 20);
        }
    }

    return 0;
}

