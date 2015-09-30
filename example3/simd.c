#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define N 1000000
#define REPEAT 500

int main (int argc, char *argv[]) {
  
  int i,j; 
  float x[N][N], y[N][N];
  
#pragma omp parallel
  {
#pragma omp for 
    for (i=0; i<N; i++) {
#pragma omp simd safelen(18)
      for (j=18; j<N-18; j++) {
	x[i][j] = x[i][j-18] + sin(y[i][j]);
	y[i][j] = y[i][j+18] + cos(x[i][j]);
      }
    }
  }

  return(EXIT_SUCCESS);
}
