/*
** Sending simple, point-to-point messages.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h" 

#define MASTER 0

int main(int argc, char* argv[])
{
  int myrank;
  int size;
  int dest;              /* destination rank for message */
  int source;            /* source rank of a message */
  int tag = 0;           /* scope for adding extra information to a message */
  MPI_Status status;     /* struct used by MPI_Recv */
  char message[BUFSIZ];

  /* MPI_Init returns once it has started up processes */
  MPI_Init( &argc, &argv );

  /* size and rank will become ubiquitous */ 
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank );

  /* 
  ** SPMD - conditionals based upon rank
  ** will also become ubiquitous
  */
  if (myrank != MASTER) {  /* this is _NOT_ the master process */
    /* create a message to send, in this case a character array */
    sprintf(message, "Come-in Danny-Boy, this is process %d!", myrank);
    /* send to the master process */
    dest = MASTER;
    /* 
    ** Send our first message!
    ** use strlen()+1, so that we include the string terminator, '\0'
    */
    MPI_Send(message,strlen(message)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

  }
  else {             /* i.e. this is the master process */
    printf("messages recieved by rank %d (master):\n", myrank);
    /* loop over all the over processes */
    for (source=1; source<size; source++) {
      /* recieving their messages.. */
      MPI_Recv(message, BUFSIZ, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
      /* and then printing them */
      printf("recieved message: %s\n", message);
    }
  }

  /* don't forget to tidy up when we're done */
  MPI_Finalize();

  /* and exit the program */
  return EXIT_SUCCESS;
}
