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
** This example works in parallel, using 'one-sided'
** (remote memory access) MPI comms.  It also takes
** one argument -- the number of trapezoids.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define sqr(x)((x)*(x))
#define MASTER 0

/* function prototypes */
double f(double x);  /* the function that we are integrating */
double trapezoid(double local_a, double local_b, int local_n, double width);

int main(int argc, char** argv) {

  int rank;           /* process rank */
  int size;           /* number of processes */
  int total_n;        /* total number of trapezoids to evaluate */
  int local_n;        /* number of trapezoids evaluated by each process */

  MPI_Win n_win, integral_win;  /* RMA windows */

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
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  /* 
  ** Set up RMA windows.
  ** Note that all processes are involved in setting up
  ** the windows, but that also, in this case, only memory
  ** belonging to the master process is put into a window.
  */
  if (rank == MASTER) {
    MPI_Win_create(&total_n, sizeof(int), 1, MPI_INFO_NULL,
		   MPI_COMM_WORLD, &n_win);
    MPI_Win_create(&integral, sizeof(double), 1, MPI_INFO_NULL,
		   MPI_COMM_WORLD, &integral_win);
  }
  else {
    MPI_Win_create(MPI_BOTTOM, 0, 1, MPI_INFO_NULL,
		   MPI_COMM_WORLD, &n_win);
    MPI_Win_create(MPI_BOTTOM, 0, 1, MPI_INFO_NULL,
		   MPI_COMM_WORLD, &integral_win);
  }

  /* MASTER reads command line */
  if (rank == MASTER) {
    if (argc<2) {
      printf("\n(1) Usage: rma_trapezoid.exe <num_trapezoids>\n");
      MPI_Abort(MPI_COMM_WORLD,1);
    }
    sscanf(argv[1],"%d",&total_n);
    if (total_n == 0) {
      MPI_Abort(MPI_COMM_WORLD,0);
    }
  }
  
  /* 
  ** Other processes 'get' the value of 'total_n' .
  ** Note that MPI_Win_fence() is a collective operation
  ** and so all processes call it.
  */
  MPI_Win_fence(0,n_win);  
  if (rank != MASTER) {
    MPI_Get(&total_n, 1, MPI_INT, MASTER, 0, 1, MPI_INT, n_win);
  }
  MPI_Win_fence(0,n_win);

  /* width is the same for all trapezoids, on all procs */
  width = (b - a)/(double)total_n;

  /* how many trapezoids per process? */
  local_n = total_n/size;

  printf("rank %d:\tlocal_n %d\n", rank, local_n);

  /* calculate local interals to work on */
  local_a = a + (rank * (local_n * width));  /* since each local interval is (local_n * width) */
  local_b = local_a + (local_n * width);

  printf("rank %d:\tlocal_a %f, local_b %f\n", rank, local_a, local_b);

  /* local trapezoid calculations are bundled into a function */ 
  local_sum = trapezoid(local_a,local_b, local_n, width);

  printf("rank %d:\tlocal_sum %f\n", rank, local_sum);

  /*
  ** Replace send and receive of local_sums with a
  ** call to the one-sided MPI_Accumulate().
  */
  MPI_Win_fence(0,integral_win);
  MPI_Accumulate(&local_sum, 1, MPI_DOUBLE, MASTER, 0, 1, MPI_DOUBLE,
		 MPI_SUM, integral_win );
  MPI_Win_fence(0,integral_win);

  /* print the result */
  if (rank == MASTER) {
    printf("\n");
    printf("Definite integral from %f to %f estimated as %f (using %d trapezoids)\n",
	   a, b, integral, total_n);
    printf("(error: %.16f)\n",fabs(integral - PI25DT));
  }

  /* free up the windows now that we have finished with them */
  MPI_Win_free(&n_win);
  MPI_Win_free(&integral_win);

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
