/*
** An example of using the trapezoidal rule to numerically 
** integrate f(x) from the points a to b.
** The rule boils down to the summation:
** [f(a)/2 +f(b)/2 + f(x1) + f(x2) + ... + f(xN)] * w
** where w is the width of each trapezoid, which is in turn
** determined by the number (N) of trapezoids we use in 
** the estimation.
** In this case we've hardwired a, b, N, and f(x).
**
** This example works in parallel, using MPI_Reduce,
** rather than each process calling MPI_Send back the MASTER.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define sqr(x)((x)*(x))
#define N 15
#define MASTER 0

/* function prototypes */
double f(double x);  /* the function that we are integrating */
double trapezoid(double local_a, double local_b, int local_n, double width);

int main(int argc, char** argv) {

  int rank;           /* process rank */
  int nprocs;         /* number of processes */
  int local_n;        /* number of trapezoids evaluated by each process */

  double integral;    /* where we'll store the result of the integration */
  double local_sum;   /* local portion of the integration summation */
  double a = 0.0;     /* left endpoint of definite integration */
  double b = 1.0;     /* right endpoint of definite integration */
  double local_a;     /* left endpoint for local chunk */
  double local_b;     /* right endpoint for local chunk */
  double width;       /* width of each trapezoid - same for all proceses*/

  double PI25DT = 3.141592653589793238462643;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

  if (nprocs != N) {
    fprintf(stderr,"Error: wrong number of processes (use %d processes)\n", N);
    MPI_Abort(MPI_COMM_WORLD,1);
  }

  /* width is the same for all trapezoids, on all procs */
  width = (b - a)/(double)N;

  /* how many trapezoids per process? */
  local_n = N/nprocs;

  printf("rank %d:\tlocal_n %d\n", rank, local_n);

  /* calculate local interals to work on */
  local_a = a + (rank * (local_n * width));  /* since each local interval is (local_n * width) */
  local_b = local_a + (local_n * width);

  printf("rank %d:\tlocal_a %f, local_b %f\n", rank, local_a, local_b);

  /* local trapezoid calculations are bundled into a function */ 
  local_sum = trapezoid(local_a,local_b, local_n, width);

  printf("rank %d:\tlocal_sum %f\n", rank, local_sum);

  /* 
  ** Collect all the local_sums using a single call--much neater!
  ** note 'MPI_SUM' is one of several possible reduction operators
  ** note also that the first two arguments cannot be the same variable
  */
  MPI_Reduce(&local_sum, &integral, 1, MPI_DOUBLE, MPI_SUM, MASTER, MPI_COMM_WORLD);

  /* print the result */
  if (rank == MASTER) {
    printf("\n");
    printf("Definite integral from %f to %f estimated as %f (using %d trapezoids)\n",
	   a, b, integral, N);
    printf("(error: %.16f)\n",fabs(integral - PI25DT));
  }

  MPI_Finalize();

  return EXIT_SUCCESS;
}

double trapezoid(double local_a, double local_b, int local_n, double width) {

  double local_sum;   /* result of (local) integration */
  double x;           /* point in the range [a,b] */
  int ii;

  /* the first two elements in our series */
  local_sum = (f(local_a) + f(local_b))/2.0;

  /* loop over the rest */ 
  x = local_a;
  for (ii=1; ii<local_n; ii++) {
    x += width;
    local_sum += f(x);
  }
  /* don't forget the final multiplier */
  local_sum *= width;

  return local_sum;
}

/* definite integral of this will estimate pi */
double f(double x) {
  return 4.0 / (1 + sqr(x));
}
