#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N 10000000
#define REPEAT 500

int main (int argc, char *argv[]) {

  int i,j; 
  int *a, *b;

  a = (int*)malloc(N*sizeof(int));
  b = (int*)malloc(N*sizeof(int));

  /*
  ** Initialise the arrays in serial.
  */
  for (i=0;i<N;i++) {
    a[i]=0;
    b[i]=rand();
  }    

  /*
  ** Access in parallel.
  */
  for(j=0; j<REPEAT; j++) {
#pragma omp parallel for
    for (i=0; i<N; i++) {
      a[i] = b[i];
    }
  }

  free(a);
  free(b);

  return(EXIT_SUCCESS);
}
