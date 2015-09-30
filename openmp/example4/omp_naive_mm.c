#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DIM1 2000
#define DIM2 2000
#define DIM3 2000

int main()
{
  int ii, jj, kk;
  int nthreads;

  double *A;
  double *B; 
  double *C;

  long dim1 = DIM1;
  long dim2 = DIM2;
  long dim3 = DIM3;

  clock_t tic, toc;
  float elapsed_time;
  double maxr;


  A = (double*)malloc(sizeof(double)*(dim1*dim2));
  B = (double*)malloc(sizeof(double)*(dim2*dim3));
  C = (double*)malloc(sizeof(double)*(dim1*dim3));

  srand(86456);
  maxr = (double)RAND_MAX;

  /* filling out in column-major order */
  for (ii = 0; ii < dim1; ii++)
    for (jj = 0; jj < dim2; jj++)
      A[ii + jj*dim1] = rand()/maxr;

  for (ii = 0; ii < dim2; ii++)
    for (jj = 0; jj < dim3; jj++)
      B[ii + jj*dim2] = rand()/maxr;

  tic = clock();

#pragma omp parallel shared(A,B,C,nthreads) private(ii,jj,kk)
  {
#pragma master
    {
      nthreads = omp_get_num_threads();
    }

#pragma omp for
    for (jj = 0; jj < dim3; jj++)
      {
	for (ii = 0; ii < dim3; ii++)
	  C[ii + jj*dim1] = 0.;
	for (kk = 0; kk < dim2; kk++)
	  for (ii = 0; ii < dim1; ii++)
	    C[ii + jj*dim1] += A[ii + kk*dim1]*B[kk + jj*dim2];
      }   
  }
  
  toc = clock();
  elapsed_time = (float)(toc - tic)/(float)CLOCKS_PER_SEC;
  elapsed_time /= (float)nthreads;

  printf("time for C(%d,%d) = A(%d,%d) B(%d,%d) is %fs (%d threads)\n",
	 dim1, dim3, dim1, dim2, dim2, dim3, elapsed_time, nthreads);

  free(A);
  free(B);
  free(C);

  return EXIT_SUCCESS;
}
