/*
** A simple example of halo-exchange for a 2d grid.
**
**                     NCOLS
**             +--------------------+
**             |                    |
**    NROWS    |                    |     NROWS
**             |                    |
**             +--------------------+
**                     NCOLS
**
** The grid will be partitioned into subgrids, used by
** each of four ranks:
**
**                       NCOLS
**                   |     |     |
**             +-----|-----|-----|-----+
**             |     |     |     |     |
**    NROWS    |     |     |     |     |    NROWS
**             |     |     |     |     |
**             +-----|-----|-----|-----+
**                   |     |     |
**                       NCOLS
**
** A pattern of communication using only column-based
** halos will be employed.  e.g. for 4 ranks:
**
**   +-----+     +-----+     +-----+     +-----+
**   ||   ||     ||   ||     ||   ||     ||   ||
** <-|| 0 || <-> || 1 || <-> || 2 || <-> || 3 || -> 
**   ||   ||     ||   ||     ||   ||     ||   ||
**   +-----+     +-----+     +-----+     +-----+
** 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"

#define NROWS 4
#define NCOLS 16
#define MASTER 0

/* function prototypes */
int calc_ncols_from_rank(int rank, int size);

int main(int argc, char* argv[])
{
  int ii,jj;             /* row and column indices for the grid */
  int kk;                /* index for looping over ranks */
  int rank;              /* the rank of this process */
  int left;              /* the rank of the process to the left */
  int right;             /* the rank of the process to the right */
  int size;              /* number of processes in the communicator */
  int tag = 0;           /* scope for adding extra information to a message */
  MPI_Status status;     /* struct used by MPI_Recv */
  int local_nrows;       /* number of rows apportioned to this rank */
  int local_ncols;       /* number of columns apportioned to this rank */
  int remote_ncols;      /* number of columns apportioned to a remote rank */
  double *w;             /* local temperature grid at time t     */
  double *sendbuf;       /* buffer to hold values to send */
  double *recvbuf;       /* buffer to hold received values */
  double *printbuf;      /* buffer to hold values for printing */

  /* 
  ** MPI_Init returns once it has started up processes
  ** Get size of cohort and rank for this process
  */
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
  if (local_ncols < 1) {
    fprintf(stderr,"Error: too many processes:- local_ncols < 1\n");
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }
    
  /*
  ** allocate space for:
  ** - the local grid with 2 extra columns added for the halos
  ** - message passing buffers
  ** - a buffer used to print local grid values
  */
  w = (double*)malloc(sizeof(double*) * local_nrows * (local_ncols + 2));
  sendbuf = (double*)malloc(sizeof(double) * local_nrows);
  recvbuf = (double*)malloc(sizeof(double) * local_nrows);
  /* The last rank has the most columns apportioned.
     printbuf must be big enough to hold this number */ 
  remote_ncols = calc_ncols_from_rank(size-1, size); 
  printbuf = (double*)malloc(sizeof(double) * (remote_ncols + 2));
  
  /*
  ** initialize the local grid (w):
  ** - core cells are set to the value of the rank
  ** - halo cells are inititalised to a -ve value
  ** note the looping bounds for index jj is modified 
  ** to accomodate the extra halo columns
  */
  for(ii=0;ii<local_nrows;ii++) {
    for(jj=0; jj<local_ncols + 2; jj++) {
      if (jj > 0 && jj < (local_ncols + 1)) 
	w[ii * (local_ncols + 2) + jj] = (double)rank;                 /* core cells */
      else if (jj == 0 || jj == (local_ncols + 1))
	w[ii * (local_ncols + 2) + jj] = -1.0;                         /* halo cells */
    }
  }

  /*
  ** Master rank prints out the initialised grid.
  ** Proceeding row-by-row:
  ** - the master rank prints it's values, including the halo
  ** - space
  ** - the values for rows from the other ranks are received
  **   by the master, and printed
  ** - ranks other than the master send their row values to the master 
  */
  if(rank == MASTER) {
    printf("NROWS: %d\nNCOLS: %d\n",NROWS,NCOLS);
    printf("Initialised grid:\n");
  }
  for(ii=0; ii < local_nrows; ii++) {
    if(rank == MASTER) {
      for(jj=0; jj < local_ncols + 2; jj++) {
	printf("%2.1f ",w[ii * (local_ncols + 2) + jj]);
      }
      printf(" ");
      for(kk=1; kk < size; kk++) { /* loop over other ranks */
	remote_ncols = calc_ncols_from_rank(kk, size);
	MPI_Recv(printbuf, remote_ncols + 2, MPI_DOUBLE, kk, tag, MPI_COMM_WORLD, &status);
	for(jj=0; jj < remote_ncols + 2; jj++) {
	  printf("%2.1f ",printbuf[jj]);
	}
	printf(" ");
      }
      printf("\n");
    }
    else {
      MPI_Send(&w[ii * (local_ncols + 2)], local_ncols + 2, MPI_DOUBLE, MASTER, tag, MPI_COMM_WORLD);
    }
  }
  if (rank == MASTER)
    printf("\n");

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
  for(ii=0; ii < local_nrows; ii++)
    sendbuf[ii] = w[ii * (local_ncols + 2) + 1];
  MPI_Sendrecv(sendbuf, local_nrows, MPI_DOUBLE, left, tag,
	       recvbuf, local_nrows, MPI_DOUBLE, right, tag,
	       MPI_COMM_WORLD, &status);
  for(ii=0; ii < local_nrows; ii++)
    w[ii * (local_ncols + 2) + local_ncols + 1] = recvbuf[ii];
  
  /* send to the right, receive from left */
  for(ii=0; ii < local_nrows; ii++)
    sendbuf[ii] = w[ii * (local_ncols + 2) + local_ncols];
  MPI_Sendrecv(sendbuf, local_nrows, MPI_DOUBLE, right, tag,
	       recvbuf, local_nrows, MPI_DOUBLE, left, tag,
	       MPI_COMM_WORLD, &status);
  for(ii=0; ii < local_nrows; ii++)
    w[ii * (local_ncols + 2)] = recvbuf[ii];
  
  /*
  ** Master rank prints out the grid after the halo-exchange
  */
  if(rank == MASTER) {
    printf("Grid after halo-exchange.\n");
    printf("Notice that:\n");
    printf(" - the core cells are unchanged\n");
    printf(" - the halo cells now have values derived\n");
    printf("   from the core cells of neighbouring ranks\n");
  }
  for(ii=0; ii < local_nrows; ii++) {
    if(rank == MASTER) {
      for(jj=0; jj < local_ncols + 2; jj++) {
	printf("%2.1f ",w[ii * (local_ncols + 2) + jj]);
      }
      printf(" ");
      for(kk=1; kk < size; kk++) { /* loop over other ranks */
	remote_ncols = calc_ncols_from_rank(kk, size);
	MPI_Recv(printbuf, remote_ncols + 2, MPI_DOUBLE, kk, tag, MPI_COMM_WORLD, &status);
	for(jj=0; jj < remote_ncols + 2; jj++) {
	  printf("%2.1f ",printbuf[jj]);
	}
	printf(" ");
      }
      printf("\n");
    }
    else {
      MPI_Send(&w[ii * (local_ncols + 2)], local_ncols + 2, MPI_DOUBLE, MASTER, tag, MPI_COMM_WORLD);
    }
  }
  if (rank == MASTER)
    printf("\n");

  /* don't forget to tidy up when we're done */
  MPI_Finalize();

  /* free up allocated memory */
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
