#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include<sys/time.h>
#include<sys/resource.h>

#include "openacc.h"

int main(int argc, char** argv) {
  int n;                  /* size of the vector */
  float a;                /* scalar */
  float *y;               /* y-vector */ 
  float *x;               /* x-vector */ 
  float *exp;             /* expected results vector */
  float eps = 0.000001f;  /* comparison tolerance */
  int ii;
  struct timeval timstr;  /* structure to hold elapsed time */
  struct rusage ru;       /* structure to hold CPU time--system and user */
  double tic,toc;         /* floating point numbers to calculate elapsed wallclock time */
  double usrtim;          /* floating point number to record elapsed user CPU time */
  double systim;          /* floating point number to record elapsed system CPU time */  

  if(argc > 1) {
    n = atoi(argv[1]);
    assert(n>0);
  } else {
    n = 100000;
  }
  
  /* Allocate the arrays */
  x = malloc(n*sizeof(float));
  y = malloc(n*sizeof(float));
  exp = malloc(n*sizeof(float));
  
  /* Initialize the arrays on the host */
  a = 4.0f;
  for (ii=0; ii<n; ii++) {
    x[ii] = (float)ii;
    y[ii] = exp[ii] = (float)ii;
  }

  /* Get expected results on the host */
  for (ii=0; ii<n; ii++) {
    exp[ii] = a*x[ii] + exp[ii];
  }
  
  /* start timing */
  gettimeofday(&timstr,NULL);
  tic=timstr.tv_sec+(timstr.tv_usec/1000000.0);

  /* Run on the accelerator */
#pragma acc kernels loop copy(y[0:n]) copyin(x[0:n]) independent
  for (ii=0; ii<n; ii++) {
    y[ii] = a*x[ii] + y[ii];
  }
  
  /* finish timing */
  gettimeofday(&timstr,NULL);
  toc=timstr.tv_sec+(timstr.tv_usec/1000000.0);
  getrusage(RUSAGE_SELF, &ru);
  timstr=ru.ru_utime;        
  usrtim=timstr.tv_sec+(timstr.tv_usec/1000000.0);
  timstr=ru.ru_stime;        
  systim=timstr.tv_sec+(timstr.tv_usec/1000000.0);

  /* Check the results */
  for (ii=0; ii<n; ii++) {
    if(fabs(y[ii] - exp[ii]) >= eps) {
      printf("ii=%d: out[ii]=%f , exp[ii]=%f\n", ii, y[ii], exp[ii]);
      printf("error found\n");
    }
  }
  
  /* print to stdout */
  printf("\niterations finished: %d\n\n", n);
  printf("Elapsed time:\t\t\t%.6lf (s)\n", toc-tic);
  printf("Elapsed user CPU time:\t\t%.6lf (s)\n", usrtim);
  printf("Elapsed system CPU time:\t%.6lf (s)\n", systim);

  /* clean up */
  free(x);
  free(y);
  free(exp);
  
  return EXIT_SUCCESS;
}
