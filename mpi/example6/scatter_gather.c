/*
** Two more collective operations..
** Common patterns are to scatter and to gather.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h" 

#define MASTER 0

int main(int argc, char* argv[])
{
  int rank;                 /* rank of process */
  int size;                 /* number of processes started */
  int message_length;       /* character count for the scattered substring(s) */ 
  char global_str[BUFSIZ];  /* original message buffer */
  char local_str[BUFSIZ];   /* send/receive message buffer */
  char reconst_str[BUFSIZ]; /* reconsituted message buffer */
  
  /* initialise processes */
  MPI_Init( &argc, &argv );
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );

  /* initialise our global character array */
  sprintf(global_str, "When shall we three meet again? In thunder, lightning, or in rain?");
  
  /* 
  ** calculate the number of chars that will be in the messages
  ** each process gets an equally sized substring
  */
  message_length = strlen(global_str) / size;

  if (strlen(global_str) % size != 0) {
    fprintf(stderr,"Error: number of processes must evenly divide string length of %lu\n", strlen(global_str));
    MPI_Abort(MPI_COMM_WORLD,1);
  }

  if (rank == MASTER) {
    printf("Message length is: %d\n", message_length); 
    printf("Total length of original string is: %d\n", (int)strlen(global_str));
  }

  /* now scatter across the cohort of the communicator */
  MPI_Scatter(global_str, message_length, MPI_CHAR,
	      local_str, message_length, MPI_CHAR,
	      MASTER, MPI_COMM_WORLD);

  /* all processes report their substring */
  printf("rank %d, fragment:\t%s\n", rank, local_str);

  /* perform a gather */
  MPI_Gather(local_str, message_length, MPI_CHAR,
	      reconst_str, message_length, MPI_CHAR,
	      MASTER, MPI_COMM_WORLD);

  /* print the result of the gather, but only on the master of course */
  if (rank == MASTER) {
    printf("rank %d, gathered:\t%s\n", rank, reconst_str); 
  }

  MPI_Finalize();

  return EXIT_SUCCESS;
}
