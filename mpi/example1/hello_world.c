/*
** A very simple example MPI program, written in C
*/

#include <stdio.h>
#include <stdlib.h>
/* "mpi.h" contains function prototypes and macros we'll need */ 
#include "mpi.h"


int main(int argc, char* argv[])
{
  int rank;               /* 'rank' of process among it's cohort */ 
  int size;               /* size of cohort, i.e. num processes started */
  int flag;               /* for checking whether MPI_Init() has been called */
  int strlen;             /* length of a character array */
  enum bool {FALSE,TRUE}; /* enumerated type: false = 0, true = 1 */  
  char hostname[MPI_MAX_PROCESSOR_NAME];  /* character array to hold hostname running process */

  /* initialise our MPI environment */
  MPI_Init( &argc, &argv );

  /* check whether the initialisation was successful */
  MPI_Initialized(&flag);
  if ( flag != TRUE ) {
    MPI_Abort(MPI_COMM_WORLD,EXIT_FAILURE);
  }

  /* determine the hostname */
  MPI_Get_processor_name(hostname,&strlen);

  /* 
  ** determine the SIZE of the group of processes associated with
  ** the 'communicator'.  MPI_COMM_WORLD is the default communicator
  ** consisting of all the processes in the launched MPI 'job'
  */
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  
  /* determine the RANK of the current process [0:SIZE-1] */
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );

  /* 
  ** make use of these values in our print statement
  ** note that we are assuming that all processes can
  ** write to the screen
  */
  printf("Hello, world; from host %s: process %d of %d\n", hostname, rank, size);

  /* finialise the MPI enviroment */
  MPI_Finalize();

  /* and exit the program */
  return EXIT_SUCCESS;
}
