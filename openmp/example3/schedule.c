#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define CHUNKSIZE   10
#define N       100

int main (int argc, char *argv[]) {

  int nthreads, tid, i, chunk;
  float a[N];
    
  chunk = CHUNKSIZE;

#pragma omp parallel shared(a,chunk) private(i,tid,nthreads)
  {
    tid = omp_get_thread_num();
    if (tid == 0)
      {
	nthreads = omp_get_num_threads();
	printf("Number of threads = %d\n", nthreads);
      }
    printf("Thread %d starting...\n",tid);

#pragma omp for schedule(static,chunk)
    for (i=0; i<N; i++)
      {
	a[i] = i * 1.0;
	printf("Thread %d: a[%d]= %f\n",tid,i,a[i]);
      }

    printf("Thread %d done.\n",tid);
  }  /* end of parallel section */

  return(EXIT_SUCCESS);
}
