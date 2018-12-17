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

int main(int argc, char* argv[])
{
  int ii;                /* generic counter */
  int myrank;            /* the rank of this process */
  int size;              /* number of processes in the communicator */
  int direction;         /* the coordinate dimension of a shift */
  int disp;              /* displacement, >1 is 'forwards', <1 is 'backwards' along a dimension */
  int north;             /* the rank of the process above this rank in the grid */
  int south;             /* the rank of the process below this rank in the grid */
  int east;              /* the rank of the process to the right of this rank in the grid */
  int west;              /* the rank of the process to the left of this rank in the grid */
  int reorder = 0;       /* an argument to MPI_Cart_create() */
  int dims[NDIMS];       /* array to hold dimensions of an NDIMS grid of processes */
  int periods[NDIMS];    /* array to specificy periodic boundary conditions on each dimension */
  int coords[NDIMS];     /* array to hold the grid coordinates for a rank */
  MPI_Comm comm_cart;    /* a cartesian topology aware communicator */

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
  ** Each rank now discovers its coordinates in the cartesian grid
  ** and prints them to the screen.
  ** Get out a pencil and paper and draw it out, e.g.
  ** for a 2-d grid of 6 processes (ranks in the boxes)
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
  MPI_Cart_coords(comm_cart, myrank, NDIMS, coords);
  MPI_Barrier(MPI_COMM_WORLD);
  printf("rank %d has coordinates (%d,%d)\n", myrank, coords[0], coords[1]);

  /*
  ** Get to know the neighbours..
  ** Each rank will now discover which rank is to the 'north', 'south', 'east' and 'west'
  ** of it in the grid and will print that information to the screen.
  ** Check whether this makes sense given the grid that you drew above.
  ** Note periodic boundary conditions.  Also be aware that MPI_Cart_shift() does
  ** not report diagonal neighbours.
  */

  /*
  ** The first coordinate dimension holds the ranks that are to the 'west' and 'east'
  ** of the current rank.
  */
  direction = 0;
  disp = 1;
  MPI_Cart_shift(comm_cart, direction, disp, &west, &east);
  /*
  ** The second coordinate dimension holds the ranks that are to the 'south' and 'north'
  ** of the current rank.
  */
  direction = 1;
  disp = 1;
  MPI_Cart_shift(comm_cart, direction, disp, &south, &north);
  MPI_Barrier(MPI_COMM_WORLD);
  printf("rank: %d\n\tnorth=%d\n\tsouth=%d\n\teast=%d\n\twest=%d\n", myrank,north,south,east,west);

  MPI_Finalize();

  /* and exit the program */
  return EXIT_SUCCESS;
}
