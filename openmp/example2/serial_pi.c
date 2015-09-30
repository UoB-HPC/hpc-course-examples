#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main (void) 
{
  /* locals */
  static long num_steps = 100000000; /* number of steps over which to estimate pi */
  double step;                       /* the step size */
  int ii;                            /* genereric counter */
  double x;                          /* intermediate value */
  double pi = 0.0;                   /* overall estimate */
  double sum = 0.0;                  /* variable to store partial sum */

  double PI25DT = 3.141592653589793238462643;

  /* step size is dependent upon the number of steps */
  step = 1.0/(double) num_steps; 

  /* main loop */
  for (ii=0; ii<num_steps; ii++) { 
    x = (ii+0.5)*step; 
    sum += 4.0/(1.0+x*x); 
  } 
  pi = step * sum; 

  printf("pi is:\t\t%.16f\n",pi);
  printf("error is:\t%.16f\n",fabs(pi - PI25DT));

  return EXIT_SUCCESS;
}
