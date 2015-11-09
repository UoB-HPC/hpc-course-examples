/*
** A simple program illustrating deadlock due to blocking MPI_Recv
** waiting for an MPI_Send on both processes of an exchange. 
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main( int argc,char* argv[] )
{
  int size;
  int rank;
  char message[BUFSIZ];    /* uninitialised, the array will contain garbage */ 
  int other;               /* the rank of the other process */
  int tag = 0;
  MPI_Status status;
  int sendfirst;           /* the value of this controls the order of our comms */

  /* standard startup procedure */
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);

  /* keep the example to a simple pairwise exchange */
  if (size != 2) {
    fprintf(stderr,"Error: wrong number of processes (use 2 processes)\n");
    MPI_Abort(MPI_COMM_WORLD,1);
  }

  /*
  ** a command line argument controls the order of
  ** the send and receive calls
  */
  if (argc < 2) {
    printf("Usage: %s  <1|0> (1=call 'send' first, 0=call 'recv' first)\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD,1);
  }
  sscanf(argv[1],"%d",&sendfirst);

  /* we require a 1 or a 0 as the argument */
  if (sendfirst != 0 && sendfirst != 1) {
    fprintf(stderr,"Error:  You must enter a '1' or a '0' (1=call 'send' first, 0=call 'recv' first)\n");
    MPI_Abort(MPI_COMM_WORLD,1);
  }
  
  /* the rank of the other process in the pair is.. */
  other = (rank+1)%2;

  /* now for the comms */
  if (sendfirst == 1) {
    printf("process %d of %d. Sending..\n",rank,size);
    /* first send.. */
    MPI_Send(message, BUFSIZ, MPI_CHAR, other, tag, MPI_COMM_WORLD);
    /* ..then receive */ 
    MPI_Recv(message, BUFSIZ, MPI_CHAR, other, tag ,MPI_COMM_WORLD, &status);
    /* 
    ** We _may_ get here.. 
    ** but only if the blocking function MPI_Send() has access to a system buffer
    */
    printf("process %d of %d. Received.\n",rank,size);  
  }
  else {
    printf("process %d of %d. Receiving..\n",rank,size);
    MPI_Recv(message, BUFSIZ, MPI_CHAR, other, tag, MPI_COMM_WORLD, &status);
    MPI_Send(message, BUFSIZ ,MPI_CHAR, other, tag, MPI_COMM_WORLD);
    /* 
    ** The blocking function MPI_Recv() cannot return and so we cannot get here..
    */ 
    printf("We'll never get here!\n");
  }
  
  MPI_Finalize();
  
  return EXIT_SUCCESS;
}
