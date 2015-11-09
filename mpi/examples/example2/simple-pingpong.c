/*
** A rudimentary implementation of ping-pong timing for message passing.
*/

#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

#define proc_A 0
#define proc_B 1
#define ping 101
#define pong 101

float buffer[10001];

int processor_A (void);
int processor_B (void);

int main ( int argc, char *argv[] )
{
  int ierror;
  int rank; 
  int size;

  /* Initialize MPI */
  MPI_Init(&argc, &argv);

  /* Get rank */
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);

  /* keep the example to a simple pairwise exchange */
  if (size != 2) {
    fprintf(stderr,"Error: wrong number of processes (use 2 processes)\n");
    MPI_Abort(MPI_COMM_WORLD,1);
  }

  /* Call ping-pong functions */
  if (rank == proc_A) {
    processor_A();
  }
  else if (rank == proc_B) {
    processor_B();
  }

  /* Finalize MPI */
  MPI_Finalize();
  
  return EXIT_SUCCESS;
}

int processor_A( void )
{
  int ii;
  int ierror;
  int sfloat;
  MPI_Status status;
  double tic, toc, elapsed_time;
  extern float buffer[10001];
  int length;

  sfloat = sizeof(float);

  printf("length   time/message (usec)    transfer rate (Gbyte/sec)\n");

  /* Process A sets the message size */
  for (length=1; length<=10001; length+=1000) { 
    /* Get the start time for the pingpong message passing */
    tic = MPI_Wtime();

    /* Process A sends and then receives the message back 1000 times */
    for (ii=0; ii<1000; ii++){
      MPI_Ssend(buffer, length, MPI_FLOAT, proc_B, ping,
		MPI_COMM_WORLD);
      MPI_Recv(buffer, length, MPI_FLOAT, proc_B, pong,
	       MPI_COMM_WORLD, &status);
    }

    /* Get the finish time for the pingpong message passing */
    toc = MPI_Wtime();
    elapsed_time = toc - tic;
    printf("%d\t      %f               %f\n", length,
	   (elapsed_time/2000.0)*1000000.0,
	   (float)(2*sfloat*1000*length)/(elapsed_time*1000000000));
  }

  return EXIT_SUCCESS;
}

int processor_B( void )
{
  int ii;
  int ierror;
  MPI_Status status;
  extern float buffer[10001];
  int length;

  /* Process B sets the message size */
  for (length=1; length<=10001; length+=1000) { 
    /* Process B receives and then sends the message back 1000 times */
    for (ii=0; ii<1000; ii++) {
      MPI_Recv(buffer, length, MPI_FLOAT, proc_A, ping,
	       MPI_COMM_WORLD, &status);
      MPI_Ssend(buffer, length, MPI_FLOAT, proc_A, pong,
		MPI_COMM_WORLD);
    }
  }

  return EXIT_SUCCESS;
}
