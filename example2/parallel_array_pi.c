#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<omp.h> 

#define NUM_THREADS 4

int main (void) 
{
  /* locals */
  static long num_steps = 100000000; /* number of steps over which to estimate pi */
  double step;                       /* the step size */
  int ii;                            /* genereric counter */
  int thread_id;                     /* ID for particular thread */
  double x,x2;                       /* intermediate values */
  double pi = 0.0;                   /* overall estimate */
  double sum[NUM_THREADS];           /* array to store partial sums */

  double PI25DT = 3.141592653589793238462643;
  
  /* step size is dependent upon the number of steps */
  step = 1.0/(double) num_steps; 
  
  /* library call to set the number of threads used */ 
  omp_set_num_threads(NUM_THREADS);

  /* threaded region */
  /* take note of which variables are declared as shared,
     and which are private */
#pragma omp parallel shared(sum,pi) private(x,x2,thread_id)
  {
    thread_id = omp_get_thread_num();
    sum[thread_id] = 0.0;
#pragma omp for
    for (ii=0; ii<num_steps; ii++) { 
      x = (ii+0.5)*step; 
      x2 = 4.0/(1.0+x*x);
      sum[thread_id] += x2;  /* note that whole array will likely be in cache */ 
    }
  } 
  
  /* back to just master thread */
  /* total the partial sums */
  for (ii=0; ii<NUM_THREADS; ii++) {
    pi += sum[ii];
  }
  pi *= step;
  
  printf("pi is:\t\t%.16f\n",pi);
  printf("error is:\t%.16f\n",fabs(pi - PI25DT));
  
  return EXIT_SUCCESS;
}
