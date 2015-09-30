#include <stdio.h>
#include <stdlib.h>

#define NX 100
#define LEFTVAL 1.0
#define RIGHTVAL 10.0
#define NSTEPS 10000

int init(double uk[], double ukpi[]);
int printValues(double uk[], int STEP);

int main(void)
{
  int ii,kk;

  double *uk = malloc(sizeof(double) * NX);
  double *ukp1 = malloc(sizeof(double) * NX);
  double *temp;

  double dx = 1.0/(double)NX;
  double dt = 0.5*dx*dx;

  init(uk, ukp1);

  for(kk=0; kk<NSTEPS; kk++) {
    for(ii=1; ii<NX-1; ii++) {
      ukp1[ii] = uk[ii] + (dt/(dx*dx))*(uk[ii+1]-2*uk[ii]+uk[ii-1]);
    }
    temp = ukp1;
    ukp1 = uk;
    uk = temp;
    printValues(uk,kk);
  }

  return EXIT_SUCCESS;
}

int init(double uk[], double ukp1[])
{
  int ii;

  uk[0] = LEFTVAL;
  uk[NX-1] = RIGHTVAL;

  for(ii=1; ii<NX-1; ii++)
    uk[ii] = 0.0;

  for(ii=0; ii<NX; ii++)
    ukp1[ii] = uk[ii];

  return EXIT_SUCCESS;
}

int printValues(double uk[], int step)
{
  int ii;
  if(step % 100 == 0) {
    printf("[");
    for(ii=0; ii<NX; ii++) {
      printf("%f ", uk[ii]);
    }
    printf("]\n");
  }
  return EXIT_SUCCESS;
}


