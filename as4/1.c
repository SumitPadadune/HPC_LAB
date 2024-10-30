#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void fun(int n){
  int arr[n];
  arr[0] = 0;
  arr[1] = 1;

  #pragma omp for schedule(dynamic, 1)
  for(int i=2; i<n; i++){
    #pragma omp critical
    {
      arr[i] = arr[i-1] + arr[i-2];
      printf("F(%d) = %d -- computed by thread %d\n", i, arr[i], omp_get_thread_num());
    }
  }

  #pragma omp barrier

  return;
}

int main(int argc, char *argv[])
{
  fun(10);
  return EXIT_SUCCESS;
}
