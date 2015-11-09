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
  MPI_Status status;     /* struct used by MPI_Recv */
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
    MPI_Send(message,strlen(message)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
  }
  else {
    tic = MPI_Wtime();
    for (source=1; source<size; source++) {
      toc = MPI_Wtime();
      printf("elapsed time %f (s):\tprocess %d, calling blocking MPI_Recv() (source rank %d)\n",
	     (toc - tic), rank, source);
      MPI_Recv(message, BUFSIZ, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
      toc = MPI_Wtime();
      printf("elapsed time %f (s):\tprocess %d, MPI_Recv() returned with message: %s\n",
	     (toc - tic), rank, message);
    }
    resolution = MPI_Wtick();
    printf("\ntiming resolution is %f (s)\n", resolution);
  }



  MPI_Finalize();
  return EXIT_SUCCESS;
}
