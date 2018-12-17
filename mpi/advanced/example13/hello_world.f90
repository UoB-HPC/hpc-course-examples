program f90_mpi_hello

  ! In Fortran90 we have "use", instead of "include 'mpi.h'" 
  use mpi

  implicit none

  integer :: procid   ! rank of the local process
  integer :: numproc  ! number of processes launched
  integer :: ierr     ! for error checking
  integer :: strlen   ! string length
  logical :: flag     ! for checking whether MPI environment
  character(len=MPI_MAX_PROCESSOR_NAME) :: hostname
  
  ! Initialise the MPI execution environment
  call MPI_INIT( ierr )

  ! Check that the MPI environment was successfully initialised
  call MPI_INITIALIZED( flag, ierr)
  if (flag .NEQV. .true.) then
     write (*,*) 'Error: MPI environment was not initialised'
     call MPI_ABORT( MPI_COMM_WORLD, 1, ierr )
  end if

  ! Find the hostname for this process
  call MPI_GET_PROCESSOR_NAME( hostname, strlen, ierr ) 

  ! Determine the size of the group of processes associated with                                                
  ! the 'communicator'.  MPI_COMM_WORLD is the default communicator                                             
  ! consisting of all the processes in the launched MPI 'job'
  call MPI_COMM_SIZE( MPI_COMM_WORLD, numproc, ierr )

  ! Determine the RANK of the calling process in the context
  ! of the communiator [0:SIZE-1]
  call MPI_COMM_RANK( MPI_COMM_WORLD, procid, ierr )

  ! Make use of these values in our print statement
  write(*,390) trim(hostname),  procid, numproc
390 format("hello, fortran90 world; from host ", &
         A, ": process ", I3, " of ", I3)

  ! Finalise our execution environment and exit 
  call MPI_FINALIZE(ierr)
  
end program f90_mpi_hello
