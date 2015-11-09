/*
** Program which splits the default communicator into 3
** non-overlapping smaller communicators.
*/

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h" 

#define NPROCS 6

int main(int argc,char* argv[])
{ 
  int rank;
  int size;
  int new_rank;             /* ranks are always contiguous, starting at 0 */
  int sendbuf;
  int recvbuf;
  int count;
  int colour;               /* subgroups are comprised of processes of the same colour */

  MPI_Group new_group;      /* new group, to be created */
  MPI_Comm  new_comm;       /* new communicator, to be created */

  /* standard startup */
  MPI_Init(&argc,&argv); 
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
  MPI_Comm_size(MPI_COMM_WORLD, &size); 

  /* need to impose of certain size of MPI_COMM_WORLD
  ** as we've hardwired the list of process ranks in
  ** the new groups */
  if(size != NPROCS) {
    fprintf(stderr,"Error: Must have %d processes in MPI_COMM_WORLD\n", NPROCS);
    MPI_Abort(MPI_COMM_WORLD,1);
  }

  /* going to broadcast my rank amongst the default communicator */
  sendbuf = rank; 
  count = 1;

  /* set the colour of the subgroups:
  ** all processes will have a value of either 0, 1 or 2 */
  colour = rank % 3;

  /* Divide processes into 3 distinct groups based upon 'colour'.
  ** Within the new communicators, the processes are relatively ranked
  ** in accordance with their rank in the default communicator.  This the
  ** thrird, 'key' argument. */ 
  MPI_Comm_split(MPI_COMM_WORLD, colour, rank, &new_comm);

  /* extract opaque group object from the new communicator */
  MPI_Comm_group(new_comm, &new_group);

  /* get rank in new group */
  MPI_Group_rank (new_group, &new_rank); 

  /* Total the ranks--as associated with MPI_COMM_WORLD--within the newer,
  ** smaller communicators */
  MPI_Allreduce(&sendbuf, &recvbuf, count, MPI_INT, MPI_SUM, new_comm); 
  
  /* all processes print to screen */
  printf("rank= %d newrank= %d recvbuf= %d\n",rank,new_rank,recvbuf); 
  
  MPI_Finalize();
  
  return EXIT_SUCCESS;
} 
