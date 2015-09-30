/*
**
*/

#include <stdlib.h>
#include <stdio.h>

#define DATA_SIZE 100000000
#define LOW_LIMIT 10000

static int partition (int p, int r, float *data)
{
  float x = data[p];
  int k = p;
  int l = r+1;
  float t;
  while (1) {
    do k++; while ((data[k] <= x) && (k < r));
    do l--; while (data[l] > x);
    while (k < l) {
      t = data[k];  data[k] = data[l];  data[l] = t;
      do k++; while (data[k] <= x);
      do l--; while (data[l] > x);
    }
    t = data[p];  data[p] = data[l];  data[l] = t;
    return l;
  }
}

static void seq_quick_sort (int p, int r, float *data)
{
  if (p < r) {
    int q = partition (p, r, data);
    seq_quick_sort (p, q-1, data);
    seq_quick_sort (q+1, r, data);
  }
}

static void par_quick_sort (int p, int r, float *data, int low_limit)
{
  if (p < r) {
    if ((r-p) < low_limit) 
      seq_quick_sort (p, r, data);
    else {
      int q = partition (p, r, data);
#pragma omp parallel sections firstprivate(data, p, q, r)
      {
#pragma omp section
	par_quick_sort (p, q-1, data, low_limit);
#pragma omp section
	par_quick_sort (q+1, r, data, low_limit);
      }
    }
  }
}

int main( int argc, char * argv[] )
{
  float* data = NULL;
  int ii;

  /* allocating space on the heap due to stack size limits */
  data = (float*)malloc(sizeof(float)*DATA_SIZE);
  
  /* seed the pseudo-random number generator
  ** with the current calendar time */
  srand(time(NULL));
  
  /* fill out the data array with random integers */
  for ( ii=0; ii<DATA_SIZE; ii++) {
    data[ii] = 1.1 * (float)rand();
  }

  /* quicksort the list of numbers */
  par_quick_sort (0, DATA_SIZE-1, data, LOW_LIMIT);

  free(data);

  return EXIT_SUCCESS;
}
