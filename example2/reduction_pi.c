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
  double x,x2;                       /* intermediate values */
  double pi = 0.0;                   /* overall estimate */
  double sum = 0.0;                  /* _reduction_ variable to store partial sums */

  double PI25DT = 3.141592653589793238462643;

  /* step size is dependent upon the number of steps */
  step = 1.0/(double) num_steps; 

  /* library call to set the number of threads used */ 
  omp_set_num_threads(NUM_THREADS);

#pragma omp parallel for reduction(+:sum) private(x,x2) 
    for (ii=0;ii<num_steps; ii++){ 
      x = (ii+0.5)*step; 
      x2 = 4.0/(1.0+x*x);
      sum = sum + x2;  /* note that sum is now in a reduction */
    } 

    /* back to master thread */
    pi = step * sum; 
    printf("pi is:\t\t%.16f\n",pi);
    printf("error is:\t%.16f\n",fabs(pi - PI25DT));
    
    return EXIT_SUCCESS;
}
