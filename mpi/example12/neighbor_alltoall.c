/*
** This program introduces Cartesian topology aware communicators
** and related functions to, e.g. determine neighbouring ranks.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h" 

#define NDIMS 2  /* setting the number of dimensions in the grid with a macro */
#define MASTER 0
#define ALPHSIZE 26

int main(int argc, char* argv[])
{
  char alphabet[ALPHSIZE] = "abcdefghijklmnopqrstuvwxyz";
  int ii;                /* generic counter */
  int myrank;            /* the rank of this process */
  int size;              /* number of processes in the communicator */
  int reorder = 0;       /* an argument to MPI_Cart_create() */
  int dims[NDIMS];       /* array to hold dimensions of an NDIMS grid of processes */
  int periods[NDIMS];    /* array to specificy periodic boundary conditions on each dimension */
  MPI_Comm comm_cart;    /* a cartesian topology aware communicator */
  char sendbuf[BUFSIZ];  /* buffer to hold data to be sent to other ranks */
  char recvbuf[BUFSIZ];  /* buffer to hold data which is recieved from other ranks */ 

  /* MPI_Init returns once it has started up processes */
  MPI_Init( &argc, &argv );

  /* size and rank will become ubiquitous */ 
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank );

  /* Some constraints on the number of dimensions and processes
  ** help to keep this example relatively simple.  If you decide
  ** to experiment, you will likely need to relax these constraints
  ** and adapt other aspects of the code.
  */
  if (NDIMS != 2) {
    fprintf(stderr,"Error: NDIMS assumed to be 2.\n");
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }
  if (size < (NDIMS * NDIMS)) {
    fprintf(stderr,"Error: size assumed to be at least NDIMS * NDIMS, i.e. 4.\n");
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }
  if ((size % 2) > 0) {
    fprintf(stderr,"Error: size assumed to be even.\n");
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }

  /* Initialise the dims and periods arrays */
  for (ii=0; ii<NDIMS; ii++) {
    dims[ii] = 0;
    periods[ii] = 1; /* set periodic boundary conditions to True for all dimensions */
  }

  /*
  ** Use a helper function to determine the dimensions of
  ** in this case a 2-d grid of processes (but you could experiment)
  ** that we will use for our calculations.  Print the results.
  */
  MPI_Dims_create(size, NDIMS, dims);
  if(myrank == MASTER) {
    printf("ranks spread over a grid of %d dimension(s): [%d,%d]\n", NDIMS, dims[0], dims[1]);
  }

  /*
  ** Create the topology aware communicator.
  */
  MPI_Cart_create(MPI_COMM_WORLD, NDIMS, dims, periods, reorder, &comm_cart);

  /*
  ** A 2-d grid of 6 processes (ranks in the boxes)
  ** where we will denote the first dimension as the X coordinate
  ** and the second dimension as the Y coordinate:
  **
  ** Y
  ** ^
  ** |
  **     --- --- ---
  ** 1  | 1 | 3 | 5 |
  **     --- --- ---
  ** 0  | 0 | 2 | 4 |
  **     --- --- ---
  **
  **      0   1   2 -> X
  */

  /*
  ** Setup the send buffer.  Each rank will send the Nth letter of the alphabet.
  ** Where N = rank. 
  */
  for (ii=0; ii<NDIMS*2; ii++) {
    sendbuf[ii] = alphabet[myrank];
  }
  sendbuf[NDIMS*2] = '\0';
  printf("rank: %d\tsendbuf: %s\n", myrank, sendbuf);

  /*
  ** Exchange.
  */
  MPI_Neighbor_alltoall(sendbuf, 1, MPI_CHAR, recvbuf, 1, MPI_CHAR, comm_cart);

  /*
  ** Examine the recieve buffer.
  */
  recvbuf[NDIMS*2] = '\0';
  printf("rank: %d\trecvbuf: %s\n", myrank, recvbuf);

  MPI_Finalize();

  /* and exit the program */
  return EXIT_SUCCESS;
}
