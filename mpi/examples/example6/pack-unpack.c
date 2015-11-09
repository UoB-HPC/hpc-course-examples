/*
** Sending fewer messages incurs less communication
** overhead.  Packing several variables into a single
** message and unpacking them at the other end can
** help reduce the number of messages sent.
** Beware, however, the cost of the packing and
** unpacking operations.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h" 

#define MASTER 0
#define STRLEN 25

int main(int argc, char* argv[])
{
  int rank;                     /* rank of process */
  int size;                     /* number of processes started */
  int position;                 /* marker used in pack and unpack operations */

  char message[BUFSIZ];         /* message to send */

  float  station_freq;          /* radio station frequency */
  char   station_name[STRLEN];  /* radio station name */
  int    station_preset_num;    /* each station has a shortcut */

  /* initialise processes */
  MPI_Init( &argc, &argv );
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );

  if (rank == MASTER) {
    /* assign values for a radio station */
    station_freq = 94.9;
    sprintf(station_name, "radio Bristol");
    station_preset_num = 1;

    position = 0;
    /* now let's pack all those values into a single message */
    MPI_Pack(&station_freq, 1, MPI_FLOAT, message, BUFSIZ,
	     &position, MPI_COMM_WORLD);
    /* position has been incremented to first free byte in the message.. */
    MPI_Pack(station_name, STRLEN, MPI_CHAR, message, BUFSIZ,
	     &position, MPI_COMM_WORLD);
    /* position has been incremented again.. */
    MPI_Pack(&station_preset_num, 1, MPI_INT, message, BUFSIZ,
	     &position, MPI_COMM_WORLD);

    /* Now that we've assembled our message, let's broadcast it */
    MPI_Bcast(message, BUFSIZ, MPI_PACKED, MASTER, MPI_COMM_WORLD);
  }
  else {
    /* matching broadcast on other processes */
    MPI_Bcast(message, BUFSIZ, MPI_PACKED, MASTER, MPI_COMM_WORLD);

    /* 
    ** followed by some unpacking of the message
    ** position is incremented as with pack
    */
    
    position = 0;
    MPI_Unpack(message, BUFSIZ, &position, &station_freq, 1,
	       MPI_FLOAT, MPI_COMM_WORLD);
    /* Note that we must know the length of string to expect here!  */
    MPI_Unpack(message, BUFSIZ, &position, station_name, STRLEN,
	       MPI_CHAR, MPI_COMM_WORLD);
    MPI_Unpack(message, BUFSIZ, &position, &station_preset_num, 1,
	       MPI_INT, MPI_COMM_WORLD);

    /* let's check what has been transmitted and unpacked */
    printf("rank %d:\t%d %.1f %s\n", rank, station_preset_num, station_freq, station_name); 
  }

  MPI_Finalize();

  return EXIT_SUCCESS;
}
