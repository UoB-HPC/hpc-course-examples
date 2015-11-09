#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define MASTER 0
#define BUFSIZE 1024

int main(int argc, char *argv[])
{
  int rank;
  int size;
  int dest;              /* destination rank for message */
  int source;            /* source rank of a message */
  int tag = 0;           /* scope for adding extra information to a message */
  MPI_Status status;     /* struct used by MPI_Recv */
  int buf;  /* buffer is a single int */
  FILE *fh;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  buf = rank * 100 + 1;  /* all ranks set buf to a unique value */

  if (rank != MASTER) {
    dest = MASTER;
    MPI_Send(&buf, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
  }
  else {
    fh = fopen("collated.txt", "w");
    source=0;
    fprintf(fh,"rank %d: %d\n", source, buf);  /* MASTER'S buf value */
    for (source=1; source<size; source++) {
      MPI_Recv(&buf, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
      fprintf(fh,"rank %d: %d\n", source, buf);
    }
    fclose(fh);
  }

  MPI_Finalize();

  return EXIT_SUCCESS;
}
