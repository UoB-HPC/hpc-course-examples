#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N       100

int main (int argc, char *argv[]) {

  int nthreads, tid, i;
  float a[N];
    
#pragma omp parallel shared(a) private(i,tid,nthreads)
  {
    tid = omp_get_thread_num();
    if (tid == 0)
      {
	nthreads = omp_get_num_threads();
	printf("Number of threads = %d\n", nthreads);
      }
    printf("Thread %d starting...\n",tid);

#pragma omp for 
    for (i=0; i<N; i++)
      {
	a[i] = i * 1.0;
	printf("Thread %d: a[%d]= %f\n",tid,i,a[i]);
      }

    printf("Thread %d done.\n",tid);
  }  /* end of parallel section */

  return(EXIT_SUCCESS);
}
