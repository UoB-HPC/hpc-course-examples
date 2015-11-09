/*
** Program to highlight group creation, and the role
** of groups in setting up communicators.
*/

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h" 

#define NPROCS 8 

int main(int argc,char* argv[])
{ 
  int rank;
  int size;
  int new_rank;             /* ranks are always contiguous, starting at 0 */
  int sendbuf;
  int recvbuf;
  int count;
  int ranks1[4]={0,1,2,3};  /* list of process ranks for 1st new group */
  int ranks2[4]={4,5,6,7};  /* list of process ranks for 2nd new group */ 

  MPI_Group orig_group;     /* opaque group object from MPI_COMM_WORLD */
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

  /* Extract the original group handle, as we can only make new
  ** groups in MPI, through reference to existing groups */ 
  MPI_Comm_group(MPI_COMM_WORLD, &orig_group);
      
  /* Divide processes into two distinct groups based upon rank.
  ** We're relying on integer division here, so let's be caseful! */ 
  if (rank < NPROCS/2) {
    MPI_Group_incl(orig_group, NPROCS/2, ranks1, &new_group);
  } 
  else { 
    MPI_Group_incl(orig_group, NPROCS/2, ranks2, &new_group);
  } 

  /* Create new communicator and then perform collective communications.
  ** Notice that all processes have called the group and communicator
  ** creation functions. */ 
  MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm); 

  /* Total the ranks--as associated with MPI_COMM_WORLD--within the newer,
  ** smaller communicators */
  MPI_Allreduce(&sendbuf, &recvbuf, count, MPI_INT, MPI_SUM, new_comm); 

  /* get rank in new group */
  MPI_Group_rank (new_group, &new_rank); 
  
  /* all processes print to screen */
  printf("rank= %d newrank= %d recvbuf= %d\n",rank,new_rank,recvbuf); 
  
  MPI_Finalize();

  return EXIT_SUCCESS;
} 
