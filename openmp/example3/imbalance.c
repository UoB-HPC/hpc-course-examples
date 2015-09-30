#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define N 100

int main (int argc, char *argv[]) {

  int nthreads, tid, i, chunk;
  float a[N];
    
#pragma omp parallel shared(a,chunk) private(i,tid,nthreads)
  {
    tid = omp_get_thread_num();
    if (tid == 0)
      {
	nthreads = omp_get_num_threads();
	printf("Number of threads = %d\n", nthreads);
      }
    printf("Thread %d starting...\n",tid);

    /*
    ** Let's deliberatly create an imbalanced workload.
    ** We'll simulate this by adding a call to sleep for
    ** a subset of the loop iterations.
    ** For example, the first 10 loop iterations below
    ** take a lot longer than the rest.
    ** Experiment with the scheduling clause, e.g.
    ** the default round-robin vs. dynamic scheduling.
    */

#pragma omp for /* schedule(dynamic) */
    for (i=0; i<N; i++)
      {
	if (i <= 10) {
	  sleep(1);
	}
	a[i] = i * 1.0;
	printf("Thread %d: a[%d]= %f\n",tid,i,a[i]);
      }

    printf("Thread %d done.\n",tid);
  }  /* end of parallel section */

  return(EXIT_SUCCESS);
}
