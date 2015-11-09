/*
** A simple, skeleton program sending point-to-point messages in
** a pattern of communication:
**
**    ---       ---       ---       ---
**   |   |     |   |     |   |     |   |
** <-| 0 | <-> | 1 | <-> | 2 | <-> | 3 | -> 
**   |   |     |   |     |   |     |   |
**    ---       ---       ---       ---
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h" 

int main(int argc, char* argv[])
{
  int myrank;              /* the rank of this process */
  int left;                /* the rank of the process to the left */
  int right;               /* the rank of the process to the right */
  int size;                /* number of processes in the communicator */
  char sendbuf[BUFSIZ];
  char recvbuf[BUFSIZ];
  MPI_Win send_win;        /* RMA window for one-sided MPI calls */

  /* MPI_Init returns once it has started up processes */
  MPI_Init( &argc, &argv );

  /* size and rank will become ubiquitous */ 
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank );

  /* example constrained to 4 processes, but code designed to work in more general cases */
  if (size != 4) {
    fprintf(stderr,"Error: this examples must be run over 4 processes\n");
    MPI_Abort(MPI_COMM_WORLD,1);
  }

  /*
  ** All ranks collectively declare an RMA window
  */
  MPI_Win_create(&sendbuf, BUFSIZ, 1, MPI_INFO_NULL,
		 MPI_COMM_WORLD, &send_win);

  /* 
  ** determine process ranks to the left and right of myrank
  ** respecting periodic boundary conditions
  */
  right = (myrank + 1) % size;
  left = (myrank == 0) ? (myrank + size - 1) : (myrank - 1);
  
  /* compose messages */
  switch (myrank) {
  case 0:
    sprintf(sendbuf, "Message from Crosby (process %d)", myrank);
    break;
  case 1:
    sprintf(sendbuf, "Message from Stills (process %d)", myrank);
    break;
  case 2:
    sprintf(sendbuf, "Message from Nash (process %d)", myrank);
    break;
  case 3:
    sprintf(sendbuf, "Message from Young (process %d)", myrank);
    break;
  default:
    sprintf(sendbuf, "Program should never reach here");
  }
  
  /*
  ** communication pattern:
  ** In this example, we'll use RMA, i.e. one-sided MPI calls.
  ** Again, this can provide a relaxed pattern.
  ** We must ensure memory syncronisation at appropriate times, 
  ** however.  We do this with a memory barrier, or 'fence'.
  ** Sometimes (i.e. on the right hardware) RMA calls can be 
  ** very efficient, as the CPU may not need to be interupted,
  ** from other calculations that it is doing, in order for data
  ** to be transferred.
  ** MPI_Get() and MPI_Put() are both available to us.  However,
  ** this pattern only logically needs one.  We will arbitrarily
  ** choose to use MPI_Get():
  ** i) get from the right
  ** ii) then get from the left
  */

  MPI_Win_fence(0,send_win);
  MPI_Get(recvbuf, BUFSIZ, MPI_CHAR, right, 0, BUFSIZ, MPI_CHAR, send_win);
  MPI_Win_fence(0,send_win);
  printf("rank %d: %s\n", myrank, recvbuf);
  MPI_Get(recvbuf, BUFSIZ, MPI_CHAR, left, 0, BUFSIZ, MPI_CHAR, send_win);
  MPI_Win_fence(0,send_win);
  printf("rank %d: %s\n", myrank, recvbuf);
  
  /* destroy the RMA window when we've finished with it*/
  MPI_Win_free(&send_win);

  /* don't forget to tidy up when we're done */
  MPI_Finalize();

  /* and exit the program */
  return EXIT_SUCCESS;
}
