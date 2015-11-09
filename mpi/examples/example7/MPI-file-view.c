/*
** MPI-1 deliberately avoided file I/O issues.
** Your choices were to arrange for each process
** to write out its own file, or to send the
** relevant information to/from the master process
** and only that process would read or write to
** a file.
** MPI-2 changes that and includes routines which
** allow multiple processes to access the same file
** (subject to any filesystem restrictions, of course.) 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h" 

#define MASTER 0
#define BUFSIZE 10       /* size of storage array we'll use on each process */
#define ALPHSIZE 26      /* how many chars in alphabet */

int main(int argc, char* argv[])
{
  int rank;                /* rank of process */
  int size;                /* number of processes started */
  int ii;                  /* simple counter */
  char buf[BUFSIZE];       /* values each process will set and write to file */
  char alphabet[ALPHSIZE] = "abcdefghijklmnopqrstuvwxyz";

  MPI_File file_handle;    /* parallel file handle */ 

  /* initialise processes */
  MPI_Init( &argc, &argv );
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );

  /* arrange for a (looping) sequence of characters */
  for (ii=0; ii<BUFSIZE; ii++) {
    buf[ii] = alphabet[(ALPHSIZE + rank) % ALPHSIZE]; 
  }

  /* all processes open the specified file for writing only */
  MPI_File_open(MPI_COMM_WORLD, "view.txt",
		MPI_MODE_CREATE | MPI_MODE_WRONLY,
		MPI_INFO_NULL, &file_handle);

  /* establish a different 'view' of the file for each process */
  MPI_File_set_view(file_handle, (rank * BUFSIZE * sizeof(char)),
		    MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);

  /* now we are clear to write to the file */
  MPI_File_write(file_handle, buf, BUFSIZE, MPI_CHAR, MPI_STATUS_IGNORE);

  /* close the file when we're done */
  MPI_File_close(&file_handle);
  
  MPI_Finalize();

  return EXIT_SUCCESS;
}
