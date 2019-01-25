
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 5

int main (int argc, char *argv[])
{

  int ii, jj, kk;
  int nthreads, tid; 
  int a[N][N][N];
  float tmp;
#ifdef DEBUG
  int debugWait = 1;
#endif

#ifdef DEBUG
  while(debugWait);
#endif

    
#pragma omp parallel private(tid,nthreads)
  {
    /* all threads */
    tid = omp_get_thread_num();

    /* master thread */
    if (tid == 0) {
      nthreads = omp_get_num_threads();
    }

#pragma omp for
    for (ii=0; ii<N; ii++) {
      for (jj=0;jj<N; jj++) {
	for (kk=0;kk<N; kk++) {
	  tmp = exp((float)kk) * exp((float)jj) / exp((float)ii); /* use some CPU cycles */
	  a[ii][jj][kk] = 9;  /* initialise */
	}
      }
    }
  }  /* end of parallel section */
  
  /* tests */
  for (ii=0; ii<N; ii++) {
    for (jj=0;jj<N; jj++) {
      for (kk=0;kk<N; kk++) {
	if (a[ii][jj][kk] != 9) {
	  printf("Error, we missed a cell: a[%d][%d][%d] = %d\n", ii, jj, kk, a[ii][jj][kk]);
	}
      }
    }
  }

  return EXIT_SUCCESS;
}
