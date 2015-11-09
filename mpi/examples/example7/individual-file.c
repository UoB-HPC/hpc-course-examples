#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define MASTER 0
#define BUFSIZE 1024

int main(int argc, char *argv[])
{
  int rank;
  int size;
  int buf;  /* buffer is a single int */
  char outfile[BUFSIZE];
  FILE *fh;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  buf = rank * 100 + 1;  /* all ranks set buf to a unique value */

  sprintf(outfile,"individual-%d.txt", rank);
  fh = fopen(outfile, "w");
  fprintf(fh,"rank %d: %d\n", rank, buf);
  fclose(fh);
  
  MPI_Finalize();

  return EXIT_SUCCESS;
}
