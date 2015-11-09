/*
** Sending simple, point-to-point messages.
** Now including a delay and a timer.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "mpi.h" 

#define MASTER 0

int main(int argc, char* argv[])
{
  int rank;
  int size;
  int dest;              /* destination rank for message */
  int source;            /* source rank of a message */
  int tag = 0;           /* scope for adding extra information to a message */
  MPI_Request request;   /* request struct used in non-blocking comms calls */      
  MPI_Status status;
  char message[BUFSIZ];
  int delay = 5;         /* a delay (s) on the senders aimed at stalling the receive */
  double tic, toc;       /* for timing: interval end points (measured in seconds) */
  double resolution;     /* for timing: resolution of timing in seconds */

  MPI_Init( &argc, &argv );
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );

  if (rank != MASTER) {
    sleep(rank*delay);
    sprintf(message, "Come-in Danny-Boy.. this is from process %d", rank);
    dest = MASTER;
    MPI_Isend(message,strlen(message)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD, &request);
    /* nothing else to do for this process */
  }
  else {
    tic = MPI_Wtime();
    for (source=1; source<size; source++) {
      toc = MPI_Wtime();
      printf("elapsed time %f (s):\tprocess %d, calling non-blocking MPI_IRecv() (source rank %d)\n",
	     (toc - tic), rank, source);
      MPI_Irecv(message, BUFSIZ, MPI_CHAR, source, tag, MPI_COMM_WORLD, &request);
      toc = MPI_Wtime();
      printf("elapsed time %f (s):\tprocess %d, non-blocking MPI_IRecv() has returned..\n",
	     (toc - tic), rank);
      printf("\t\t\t\t\t..but message not yet received..\n");
      MPI_Wait(&request, &status);  /* unsafe to proceed beyond here until request has been satisfied */ 
      toc = MPI_Wtime();
      printf("elapsed time %f (s):\tprocess %d, receive request satisfied with message: %s\n", 
	     (toc - tic), rank, message);
    }
    resolution = MPI_Wtick();
    printf("\ntiming resolution is %f (s)\n", resolution);
  }

  MPI_Finalize();
  return EXIT_SUCCESS;
}
