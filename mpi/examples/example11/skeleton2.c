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
  int myrank;            /* the rank of this process */
  int left;              /* the rank of the process to the left */
  int right;             /* the rank of the process to the right */
  int size;              /* number of processes in the communicator */
  int tag = 0;           /* scope for adding extra information to a message */
  MPI_Status status;     /* struct used by MPI_Recv */
  char sendbuf[BUFSIZ];
  char recvbuf[BUFSIZ];

  /* MPI_Init returns once it has started up processes */
  MPI_Init( &argc, &argv );

  /* size and rank will become ubiquitous */ 
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank );

  /* example constrained to 4 processes, but code designed to work in more general cases */
  if (size > 4) {
    fprintf(stderr,"Error: this examples must be run over 4 processes\n");
    MPI_Abort(MPI_COMM_WORLD,1);
  }

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
  ** we will use MPI_Sendrecv() is this case
  ** all ranks will:
  ** i) send to the left and receive from the right
  ** ii) then send to the right and recieve from the left
  */
  MPI_Sendrecv(sendbuf, strlen(sendbuf)+1, MPI_CHAR, left, tag,
	      recvbuf, BUFSIZ, MPI_CHAR, right, tag, MPI_COMM_WORLD, &status);
  printf("rank %d: %s\n", myrank, recvbuf);
  MPI_Sendrecv(sendbuf, strlen(sendbuf)+1, MPI_CHAR, right, tag,
	      recvbuf, BUFSIZ, MPI_CHAR, left, tag, MPI_COMM_WORLD, &status);
  printf("rank %d: %s\n", myrank, recvbuf);

  /* don't forget to tidy up when we're done */
  MPI_Finalize();

  /* and exit the program */
  return EXIT_SUCCESS;
}
