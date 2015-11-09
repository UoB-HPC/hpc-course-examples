/*
** An MPI program to estimate pi using the dartboard
** (monte-carlo) algorithm.
**
** Imagine a circle inscribed inside a square.
** The area of the circle is, of course: A-circ = pi * sqr(r).
** But we're trying to find pi, so we re-arrange to get:
** pi = A-circ / sqr(r)  -- (eqn 1).
** We know that sqr(r) is the area of one quarter of the square,
** so A-sq = 4 * sqr(r).
** Re-arranging again, we get:
** sqr(r) = A-sq / 4  -- (eqn 2).
** We can subsitute (eqn 2) into (eqn 1) to get:
** pi = 4 * A-circ / A-sq. 
**
** Lastly, if we assume the darts land randomly somewhere inside
** the square, i.e. sometimes within the cicle, and sometimes
** outside the circle, then we can substitute the ratio of areas
** with a ratio of dart counts, i.e. a count of the darts which 
** fell inside the cicrle over a count of those which fell inside
** the square (all of them).
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mpi.h"

#define NDARTS 5000    /* number of throws at dartboard */
#define ROUNDS 10       /* number of times we throw NDARTS */
#define MASTER 0        /* task ID of master task */

double throw_darts (int nthrows);

#define sqr(x)((x)*(x))

int main(int argc, char **argv) 
{  

  double local_pi;        /* value of pi calculated by current task */
  double pi;              /* average of pi after "darts" are thrown */
  double avepi;           /* average pi value for all iterations */
  double pirecv;          /* pi received from worker */
  double pisum;           /* sum of workers pi values */
  double PI25DT = 3.141592653589793238462643;

  int rank;               /* task ID - also used as seed number */
  int nproc;              /* number of tasks */
  int tag;                /* message tag */
  int i;
  int n;

  MPI_Status  status;

  /* Obtain number of tasks and task ID */
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);

  /* Set seed for random number generator equal to rank */
  srand (rank);

  avepi = 0;

  for (i = 0; i < ROUNDS; i++) {
    /* All tasks calculate pi using dartboard algorithm */
    local_pi = throw_darts(NDARTS);
    
    /* Workers send local_pi to master */
    /* - Message type will be set to the iteration count */
    if (rank != MASTER) {
      tag = i;
      MPI_Send(&local_pi, 1, MPI_DOUBLE, MASTER, tag, MPI_COMM_WORLD);
    }
    
    /* Master receives messages from all workers */
    /* - Message type will be set to the iteration count */
    /*   a message can be received from any task, as long as the */ 
    /*   message types match */
    /* - The return code will be checked, and a message displayed */
    /*   if a problem occurred */
    else {
      tag = i;
      pisum = 0;
      for (n = 1; n < nproc; n++) {
	MPI_Recv(&pirecv, 1, MPI_DOUBLE, MPI_ANY_SOURCE, tag,
		 MPI_COMM_WORLD, &status);
	/* keep running total of pi */
	pisum += pirecv;
      }
      /* 
      ** Master calculates the average value of pi for this iteration
      ** not forgetting the Master's contribution
      */
      pi = (pisum + local_pi)/nproc;
      /* Master calculates the average value of pi over all iterations */
      avepi = ((avepi * i) + pi)/(i + 1);  
      printf("   After %3d throws, average value of pi = %10.8f (error is %.16f)\n",
	     (NDARTS * (i + 1)),avepi, fabs(avepi - PI25DT));
    }     
  }  
  MPI_Finalize();

  return EXIT_SUCCESS;
}

double throw_darts(int nthrows)
{
  double x_coord;       /* x coordinate, between -1 and 1  */
  double y_coord;       /* y coordinate, between -1 and 1  */
  double pi;            /* pi  */
  double r;             /* random number between 0 and 1  */
  int score = 0;        /* number of darts that hit circle */
  int n;

  /* "throw darts at board" */
  for (n = 1; n <= nthrows; n++) {
    /* generate random numbers for x and y coordinates */
    r = (double)rand()/RAND_MAX;
    x_coord = (2.0 * r) - 1.0;
    r = (double)rand()/RAND_MAX;
    y_coord = (2.0 * r) - 1.0;
    
    /* if dart lands in circle, increment score */
    if ((sqr(x_coord) + sqr(y_coord)) <= 1.0)
      score++;
  }
  
  /* calculate pi */
  pi = 4.0 * (double)score/(double)nthrows;
  return(pi);
} 
