#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DIM1 2000
#define DIM2 2000
#define DIM3 2000

#define DGEMM dgemm_

int main()
{
  int i, j, k;

  double *A;
  double *B; 
  double *C;

  long dim1 = DIM1;
  long dim2 = DIM2;
  long dim3 = DIM3;

  char transa = 'N';
  char transb = 'N';

  double alpha = 1.0;
  double beta  = 0.0;

  clock_t tic, toc;
  double maxr;

  A = (double*)malloc(sizeof(double)*(dim1*dim2));
  B = (double*)malloc(sizeof(double)*(dim2*dim3));
  C = (double*)malloc(sizeof(double)*(dim1*dim3));

  srand(86456);
  maxr = (double)RAND_MAX;

  for (i = 0; i < dim1; i++)
    for (j = 0; j < dim2; j++)
      A[i + j*dim1] = rand()/maxr;

  for (i = 0; i < dim2; i++)
    for (j = 0; j < dim3; j++)
      B[i + j*dim2] = rand()/maxr;

  tic = clock();

  DGEMM(&transa,&transb,&dim1,&dim3,&dim2,&alpha,A,&dim1,B,&dim2,&beta,C,&dim1);

  toc = clock();

  printf("time for C(%d,%d) = A(%d,%d) B(%d,%d) is %fs\n",
	 dim1, dim3, dim1, dim2, dim2, dim3,((float)toc -(float)tic)/(float)CLOCKS_PER_SEC);

  free(A);
  free(B);
  free(C);

  return EXIT_SUCCESS;
}
