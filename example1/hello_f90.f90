program hello_world

  use omp_lib

  implicit none
  
  ! locals
  integer ::  nthreads, tid
  
  ! Fork a team of threads giving them their own copies of variables
  ! *NB* No space between '!' and '$omp' below...
  !$omp parallel private(nthreads, tid)

  ! Obtain thread number
  tid = omp_get_thread_num()
  print*, 'Hello, world from thread = ', tid

  ! Only master thread does this
  !$omp barrier
  if (tid .eq. 0) then
     nthreads = omp_get_num_threads()
     print*, 'Number of threads = ', nthreads
  end if
    
  ! All threads join master thread and disband
  !$omp end parallel
  
end program hello_world
