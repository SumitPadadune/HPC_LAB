#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[])
{
  char* arr[] = {"Siddharth", "Shrinivas", "Avishkar", "Rupali", "Shamrao", "Kamal"};
  #pragma omp parallel for 
  for (int i=0; i<6; i++) {
    printf("%s from thread %d \n", arr[i], omp_get_thread_num());
  }
  return EXIT_SUCCESS;
}
