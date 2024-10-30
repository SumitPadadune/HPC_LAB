#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[])
{ 
  int a = 0;
  #pragma omp parallel
  {
    printf("Thread number : %d\n", omp_get_thread_num());
    printf("Hello World: %d\n", ++a);
  }
  return 0;
}
