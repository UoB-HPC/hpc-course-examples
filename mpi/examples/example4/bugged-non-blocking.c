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
  }
  else {
    tic = MPI_Wtime();
    for (source=1; source<size; source++) {
      toc = MPI_Wtime();
      printf("elapsed time %f (s):\tprocess %d, calling non-blocking MPI_IRecv() (source rank %d)\n",
	     (toc - tic), rank, source);
      MPI_Irecv(message, BUFSIZ, MPI_CHAR, source, tag, MPI_COMM_WORLD, &request);
      /* we are missing the safety net of the MPI_Wait() */
      toc = MPI_Wtime();
      printf("elapsed time %f (s):\tMPI_Irecv() has returned, but message buffer contains: %s\n",
	     (toc - tic), message);
    }
    resolution = MPI_Wtick();
    printf("\ntiming resolution is %f (s)\n", resolution);
  }

  /* Problem: We get to here before any messages have been recieved */
  MPI_Abort(MPI_COMM_WORLD,EXIT_FAILURE);

  MPI_Finalize();
  return EXIT_SUCCESS;
}
