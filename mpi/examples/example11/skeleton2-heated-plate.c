/*
** A simple example of halo-exchange for a 2d grid.
** The example is heat diffusion on a heated plate
**
** Boundary conditions for the full grid are:
**
**                      W = 0
**             +--------------------+
**             |                    |
**    W = 100  |                    | W = 100
**             |                    |
**             +--------------------+
**                     W = 100
**
** i.e. 3 sides are held at 100 degress, while the fourth
** is held at 0 degrees.
**
** The grid will be partitioned into 4 subgrids, used by
** each of four ranks:
**
**                       W = 0
**                   |     |     |
**             +-----|-----|-----|-----+
**             |     |     |     |     |
**    W = 100  |     |     |     |     | W = 100
**             |     |     |     |     |
**             +-----|-----|-----|-----+
**                   |     |     |
**                      W = 100
**
** A pattern of communication using only column-based
** halos will be employed, e.g. for 4 ranks:
**
**   +-----+     +-----+     +-----+     +-----+
**   ||   ||     ||   ||     ||   ||     ||   ||
** <-|| 0 || <-> || 1 || <-> || 2 || <-> || 3 || -> 
**   ||   ||     ||   ||     ||   ||     ||   ||
**   +-----+     +-----+     +-----+     +-----+
**
** 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"

#define NROWS 4
#define NCOLS 16
#define EPSILON 0.01
#define ITERS 18
#define MASTER 0

/* function prototypes */
int calc_ncols_from_rank(int rank, int size);

int main(int argc, char* argv[])
{
  int ii,jj;             /* row and column indices for the grid */
  int kk;                /* index for looping over ranks */
  int start_col,end_col; /* rank dependent looping indices */
  int iter;              /* index for timestep iterations */ 
  int rank;              /* the rank of this process */
  int left;              /* the rank of the process to the left */
  int right;             /* the rank of the process to the right */
  int size;              /* number of processes in the communicator */
  int tag = 0;           /* scope for adding extra information to a message */
  MPI_Status status;     /* struct used by MPI_Recv */
  int local_nrows;       /* number of rows apportioned to this rank */
  int local_ncols;       /* number of columns apportioned to this rank */
  int remote_ncols;      /* number of columns apportioned to a remote rank */
  double boundary_mean;  /* mean of boundary values used to initialise inner cells */
  double **u;            /* local temperature grid at time t - 1 */
  double **w;            /* local temperature grid at time t     */
  double *sendbuf;       /* buffer to hold values to send */
  double *recvbuf;       /* buffer to hold received values */
  double *printbuf;      /* buffer to hold values for printing */

  /* MPI_Init returns once it has started up processes */
  /* get size and rank */ 
  MPI_Init( &argc, &argv );
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );

  /* 
  ** determine process ranks to the left and right of rank
  ** respecting periodic boundary conditions
  */
  left = (rank == MASTER) ? (rank + size - 1) : (rank - 1);
  right = (rank + 1) % size;

  /* 
  ** determine local grid size
  ** each rank gets all the rows, but a subset of the number of columns
  */
  local_nrows = NROWS;
  local_ncols = calc_ncols_from_rank(rank, size);

  /*
  ** allocate space for:
  ** - the local grid (2 extra columns added for the halos)
  ** - we'll use local grids for current and previous timesteps
  ** - buffers for message passing
  */
  u = (double**)malloc(sizeof(double*) * local_nrows);
  for(ii=0;ii<local_nrows;ii++) {
    u[ii] = (double*)malloc(sizeof(double) * (local_ncols + 2));
  }
  w = (double**)malloc(sizeof(double*) * local_nrows);
  for(ii=0;ii<local_nrows;ii++) {
    w[ii] = (double*)malloc(sizeof(double) * (local_ncols + 2));
  }
  sendbuf = (double*)malloc(sizeof(double) * local_nrows);
  recvbuf = (double*)malloc(sizeof(double) * local_nrows);
  /* The last rank has the most columns apportioned.
     printbuf must be big enough to hold this number */ 
  remote_ncols = calc_ncols_from_rank(size-1, size); 
  printbuf = (double*)malloc(sizeof(double) * (remote_ncols + 2));
  
  /*
  ** initialize the local grid for the present time (w):
  ** - set boundary conditions for any boundaries that occur in the local grid
  ** - initialize inner cells to the average of all boundary cells
  ** note the looping bounds for index jj is modified 
  ** to accomodate the extra halo columns
  ** no need to initialise the halo cells at this point
  */
  boundary_mean = ((NROWS - 2) * 100.0 * 2 + (NCOLS - 2) * 100.0) / (double) ((2 * NROWS) + (2 * NCOLS) - 4);
  for(ii=0;ii<local_nrows;ii++) {
    for(jj=1;jj<local_ncols + 1;jj++) {
      if(ii == 0)
	w[ii][jj] = 0.0;
      else if(ii == local_nrows-1)
	w[ii][jj] = 100.0;
      else if((rank == 0) && jj == 1)                  /* rank 0 gets leftmost subrid */
	w[ii][jj] = 100.0;
      else if((rank == size - 1) && jj == local_ncols) /* rank (size - 1) gets rightmost subrid */
	w[ii][jj] = 100.0;
      else
	w[ii][jj] = boundary_mean;
    }
  }

  /*
  ** time loop
  */
  for(iter=0;iter<ITERS;iter++) {
    /*
    ** halo exchange for the local grids w:
    ** - first send to the left and receive from the right,
    ** - then send to the right and receive from the left.
    ** for each direction:
    ** - first, pack the send buffer using values from the grid
    ** - exchange using MPI_Sendrecv()
    ** - unpack values from the recieve buffer into the grid
    */

    /* send to the left, receive from right */
    for(ii=0;ii<local_nrows;ii++)
      sendbuf[ii] = w[ii][1];
    MPI_Sendrecv(sendbuf, local_nrows, MPI_DOUBLE, left, tag,
		 recvbuf, local_nrows, MPI_DOUBLE, right, tag,
		 MPI_COMM_WORLD, &status);
    for(ii=0;ii<local_nrows;ii++)
      w[ii][local_ncols + 1] = recvbuf[ii];

    /* send to the right, receive from left */
    for(ii=0;ii<local_nrows;ii++)
      sendbuf[ii] = w[ii][local_ncols];
    MPI_Sendrecv(sendbuf, local_nrows, MPI_DOUBLE, right, tag,
		 recvbuf, local_nrows, MPI_DOUBLE, left, tag,
		 MPI_COMM_WORLD, &status);
    for(ii=0;ii<local_nrows;ii++)
      w[ii][0] = recvbuf[ii];

    /*
    ** copy the old solution into the u grid
    */ 
    for(ii=0;ii<local_nrows;ii++) {
      for(jj=0;jj<local_ncols + 2;jj++) {
	u[ii][jj] = w[ii][jj];
      }
    }

    /*
    ** compute new values of w using u
    ** looping extents depend on rank, as we don't
    ** want to overwrite any boundary conditions
    */
    for(ii=1;ii<local_nrows-1;ii++) {
      if(rank == 0) {
	start_col = 2;
	end_col = local_ncols;
      }
      else if(rank == size -1) {
	start_col = 1;
	end_col = local_ncols - 1;
      }
      else {
	start_col = 1;
	end_col = local_ncols;
      }
      for(jj=start_col;jj<end_col + 1;jj++) {
	w[ii][jj] = (u[ii - 1][jj] + u[ii + 1][jj] + u[ii][jj - 1] + u[ii][jj + 1]) / 4.0;
      }
    }
  }
  
  /*
  ** at the end, write out the solution.
  ** for each row of the grid:
  ** - rank 0 first prints out its cell values
  ** - then it receives row values sent from the other
  **   ranks in order, and prints them.
  */
  if(rank == MASTER) {
    printf("NROWS: %d\nNCOLS: %d\n",NROWS,NCOLS);
    printf("Final temperature distribution over heated plate:\n");
  }

  for(ii=0;ii<local_nrows;ii++) {
    if(rank == 0) {
      for(jj=1;jj<local_ncols + 1;jj++) {
	printf("%6.2f ",w[ii][jj]);
      }
      for(kk=1;kk<size;kk++) { /* loop over other ranks */
	remote_ncols = calc_ncols_from_rank(kk, size);
	MPI_Recv(printbuf,remote_ncols + 2,MPI_DOUBLE,kk,tag,MPI_COMM_WORLD,&status);
	for(jj=1;jj<remote_ncols + 1;jj++) {
	  printf("%6.2f ",printbuf[jj]);
	}
      }
      printf("\n");
    }
    else {
      MPI_Send(w[ii],local_ncols + 2,MPI_DOUBLE,MASTER,tag,MPI_COMM_WORLD);
    }
  }

  if(rank == MASTER)
    printf("\n");

  /* don't forget to tidy up when we're done */
  MPI_Finalize();

  /* free up allocated memory */
  for(ii=0;ii<local_nrows;ii++) {
    free(u[ii]);
    free(w[ii]);
  }
  free(u);
  free(w);
  free(sendbuf);
  free(recvbuf);
  free(printbuf);

  /* and exit the program */
  return EXIT_SUCCESS;
}

int calc_ncols_from_rank(int rank, int size)
{
  int ncols;

  ncols = NCOLS / size;       /* integer division */
  if ((NCOLS % size) != 0) {  /* if there is a remainder */
    if (rank == size - 1)
      ncols += NCOLS % size;  /* add remainder to last rank */
  }
  
  return ncols;
}
