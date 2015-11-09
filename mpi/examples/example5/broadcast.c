/*
** Let's compare times for send/recv pairings and broadcast.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h" 

/*#define DEBUG*/
#define MASTER 0
#define MAX_MSG 1000000

int main(int argc, char* argv[])
{
  int rank;                /* rank of process */
  int size;                /* number of processes started */
  int dest;                /* destination rank for message */
  int source = MASTER;     /* source rank of a message */
  int tag = 0;             /* scope for adding extra information to a message */
#ifdef DEBUG
  int recvd_count;         /* number of items of type MPI_Datatype received */  
#endif
  double message[MAX_MSG]; /* message to send */
  double tic, toc;         /* values for timing a section of code */
  double resolution;       /* for timing: resolution of timing in seconds */

  MPI_Status status;       /* struct used by MPI_Recv */

  /* initialise processes */
  MPI_Init( &argc, &argv );
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );

  /* 
  ** Individually send a message to all the other processes
  ** in a point-to-point fashion, for a timing comparison.
  */

  if (rank == MASTER) {
    /* start timing */
    tic =  MPI_Wtime();
    /* send messages synchronously using MPI_Ssend() */
    for (dest=1; dest<size; dest++) {
      MPI_Ssend(message, MAX_MSG, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
    }
    /* stop timing */
    toc =  MPI_Wtime();
  }
  else {
    /* notice the use of MPI_ANY_TAG as a wildcard */
    MPI_Recv(message, MAX_MSG, MPI_DOUBLE, source, MPI_ANY_TAG, 
	     MPI_COMM_WORLD, &status);
    /*
    ** Uncomment the #define at the top of the file if you
    ** would like to verify the size of the received message.
    */
#ifdef DEBUG
    MPI_Get_count(&status, MPI_DOUBLE, &recvd_count);
    printf("rank %d:\t%d items received\n", rank, recvd_count);
#endif
  }

  /* report timings for point to point */
  if (rank == MASTER) {
    printf("time (s) to send all the messages point-to-point = %f\n", (toc - tic)); 
  }
  
  /*
  ** Now, send the same information, but using a broadcast.
  */
  tic =  MPI_Wtime();
  MPI_Bcast(message, MAX_MSG, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
  toc =  MPI_Wtime();
  
  /* report timings for broadcast */
  if (rank == MASTER) {
    printf("time (s) to broadcast the message = %f\n", (toc - tic)); 
    resolution = MPI_Wtick();
    printf("\ntiming resolution is %f (s)\n", resolution);
  }

  MPI_Finalize();
  return EXIT_SUCCESS;
}
