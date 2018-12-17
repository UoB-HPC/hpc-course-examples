/*
** Sending fewer messages incurs less communication
** overhead.  Here we set up an MPI derived type
** (in fact a list of types and memory address displacements)
** so that we can bundle-up several items together.
** Using a derived type is more efficient than pack & unpack
** if you are going to make use of the container more than once. 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h" 

#define MASTER 0
#define STRLEN 25
#define NTYPES 3  /* the number of intrinsic types in our derived type */ 

int main(int argc, char* argv[])
{
  int rank;                       /* rank of process */
  int size;                       /* number of processes started */

  float  station_freq;            /* radio station frequency */
  char   station_name[STRLEN];    /* radio station name */
  int    station_preset_num;      /* each station has a shortcut */

  int block_lengths[NTYPES];      /* num of each type in a 'block' of the derived type */
  MPI_Aint displacements[NTYPES]; /* associated memory displacements for each block */
  MPI_Datatype typelist[NTYPES];  /* the actual intrinsic types comprising our bundle */

  MPI_Aint base_address;         /* used for calculating memory displacments */
  MPI_Aint address;               /* Note MPI_Aint allows for especially large displacements */

  MPI_Datatype my_dev_type;       /* the new type we'll make */


  /* initialise processes */
  MPI_Init( &argc, &argv );
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );

  /* we can fill out our block lengths without difficulty */
  block_lengths[0] = block_lengths[2] = 1;
  block_lengths[1] = STRLEN;
  
  /* same goes for our initrinsic types */
  typelist[0] = MPI_FLOAT;
  typelist[1] = MPI_CHAR;
  typelist[2] = MPI_INT;
  
  /* now we need to work out the displacements */
  displacements[0] = 0;
  
  /* MPI_Address is used since pointer arithmetic is strictly
     only allowed for members of the same array */
  MPI_Address(&station_freq, &base_address);
  MPI_Address(&station_name, &address);
  displacements[1] = address - base_address;
  
  MPI_Address(&station_preset_num, &address);
  displacements[2] = address - base_address;
  
  /* now we have what we need to build the derived type */
  MPI_Type_struct(NTYPES, block_lengths, displacements,
		  typelist, &my_dev_type);
  
  /* when we commit it, we tell the system we'll be using it */
  MPI_Type_commit(&my_dev_type);

  /* 
  ** The above is abstract in that no actual values have been
  ** assigned to the variables.  Only MASTER needs to do that.
  */
  if (rank == MASTER) {
    station_freq = 91.5;
    sprintf(station_name, "radio 3");
    station_preset_num = 3;
  }  

  /* broadcast is collective operation */
  MPI_Bcast(&station_freq, 1, my_dev_type, MASTER, MPI_COMM_WORLD);
  
  if (rank != MASTER) {
    /* let's check what has been transmitted  */
    printf("rank %d:\t%d %.1f %s\n", rank, station_preset_num, station_freq, station_name); 
  }
  
  MPI_Finalize();

  return EXIT_SUCCESS;
}
