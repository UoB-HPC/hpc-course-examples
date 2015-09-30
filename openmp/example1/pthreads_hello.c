/*
** A short example program using posix threads. 
** This can be contrasted with an equivalent OpenMP example.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 3  /* in addition to the Master thread.. */

/* 
** this function will be executed by each thread, once initialised
*/
void* run(void *threadID)
{
  printf("Hello, world from pthread %ld\n", (long)threadID);
  pthread_exit(NULL);
}

/*
** main
*/
int main(int argc, char** argv)
{
  long ii;
  int retval;
  pthread_t threads[NUM_THREADS];  /* to hold opaque thread IDs */

  /* 
  ** Spawn the additional threads, and run the given routine.
  ** Also, check whether the thread was successfully created each time.
  */
  for (ii=0; ii<NUM_THREADS; ii++) {
    retval = pthread_create(&threads[ii], NULL, run, (void *)ii);
    if (retval != 0) {
      fprintf(stderr,"ERROR, pthread_create() returned: %d\n", retval);
      exit(EXIT_FAILURE);
    }
  }
  printf("Hello, world from the MASTER thread\n");
  pthread_exit(NULL);

  return EXIT_SUCCESS;
}
