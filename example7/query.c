#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
  
  int i;
  int tid;
  int nthreads;
  int ndevices;
  int device_num;

  /* host */
#pragma omp parallel               /* open a parallel region */
  {
#pragma omp master                 /* only the master thread need query */
    {
      nthreads = omp_get_num_threads();
      printf("Host\tNumber of threads = %d\n", nthreads);
    }
  }
  
  /* determine the number of accelerators available */
  ndevices = omp_get_num_devices();
  printf("Number of accelerators = %d\n", ndevices);

  /* report the number of threads available on each device */
  for(i=0; i<ndevices; i++)
    {
#pragma omp target device(i)       /* set the target */
#pragma omp parallel               /* open a parallel region */
      {
#pragma omp master                 /* only the master thread need query */
	{
	  nthreads = omp_get_num_threads();
	  printf("Device number = %d\tNumber of threads = %d\n", i, nthreads);
	}
      }
    }
  
  return EXIT_SUCCESS;  
}
