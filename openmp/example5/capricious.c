/*
 * A simple C program that uses two OpenMP sections to create a 'race-condition'.
 * (see e.g. http://en.wikipedia.org/wiki/Race_condition for a definition).
 *
 * Two /possibly/ concurrent loops are used to fill-out the  

 * The loop counter, ii, is--by default--a shared variable and so the two
 * concurrent loops are both free to use and alter ii at the same time.
 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 2

int main(void)
{
  int ii;
  int tid;
  const int arraySize = 30;
  struct mystruct {
    char port;
    char starboard;
  };
  struct mystruct flotilla[arraySize];
#ifdef DEBUG
  int debugWait = 1;
#endif

#ifdef DEBUG
  while(debugWait);
#endif

  {
#pragma omp parallel sections num_threads(NUM_THREADS) private(tid)
    {
#pragma omp section
      {
	tid = omp_get_thread_num();
	for (ii=0; ii<arraySize; ++ii) {
	  flotilla[ii].port = 'p';
#ifdef PRINT
	  /* Q. What's the benefit of writing to stderr? */
	  fprintf(stderr, "thread %d, ii = %d\n", tid, ii);
#endif
	}
      }
#pragma omp section
      {
	tid = omp_get_thread_num();
	for (ii=0; ii<arraySize; ++ii) {
	  flotilla[ii].starboard = 's';
#ifdef PRINT
	  fprintf(stderr, "thread %d, ii = %d\n", tid, ii);
#endif
	}
      }
    }
  }
  
  /* checking section */
  for (ii=0; ii<arraySize; ++ii) {
    if (flotilla[ii].port != 'p' || flotilla[ii].starboard != 's') {
      printf("OpenMP Error! flotilla[%d].port = %c, flotilla[%d].starboard = %c\n",
	     ii, flotilla[ii].port, ii, flotilla[ii].starboard);
      break;
    }
  }
  if (ii == arraySize) {
    printf("OK!\n");
    return(EXIT_SUCCESS);
  }
  else {
    return(EXIT_FAILURE);
  }
}

