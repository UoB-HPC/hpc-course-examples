#include <iostream>
#include <cstdlib>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[]) {
  
  int rank;               /* 'rank' of process among it's cohort */ 
  int size;               /* size of cohort, i.e. num processes started */
  bool flag;              /* for checking whether MPI_Init() has been called */
  int strlen;             /* length of a character array */
  char hostname[MPI_MAX_PROCESSOR_NAME];  /* character array to hold hostname running process */
  
  MPI::Init(argc, argv);

  /* check whether the initialisation was successful */
  flag = MPI::Is_initialized();
  if ( flag != true ) {
    MPI::COMM_WORLD.Abort(EXIT_FAILURE);
  }

  /* determine the hostname */
  MPI::Get_processor_name(hostname,strlen);

  rank = MPI::COMM_WORLD.Get_rank();
  size = MPI::COMM_WORLD.Get_size();

  cout << "hello, world; from host " << hostname << " process " << rank << " of " << size << endl;

  MPI::Finalize();

  return EXIT_SUCCESS;
} 
