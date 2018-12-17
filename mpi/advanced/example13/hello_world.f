      program f77_mpi_hello

      implicit none

C 'mpi.h' contains subroutines and macros that we'll need
      include 'mpif.h'

C procid : ID of current process
C numproc : number of processes
      integer procid, numproc, strlen, ierr
      logical flag
      character(MPI_MAX_PROCESSOR_NAME) hostname

C Initialise the MPI execution environment
      call MPI_INIT( ierr )

C Check the initialisation
      call MPI_INITIALIZED( flag, ierr )
      if ( flag .NEQV. .TRUE. ) then
         call MPI_ABORT( MPI_COMM_WORLD, 1 )
      end if

C Detrmine the processor name
      call MPI_GET_PROCESSOR_NAME( hostname, strlen, ierr)

C Determine the size of the group of processes associated with                                                
C the 'communicator'.  MPI_COMM_WORLD is the default communicator                                             
C consisting of all the processes in the launched MPI 'job'
      call MPI_COMM_SIZE( MPI_COMM_WORLD, numproc, ierr )

C Determine the RANK of the calling process in the context
C of the communiator [0:SIZE-1]
      call MPI_COMM_RANK( MPI_COMM_WORLD, procid, ierr )

C Make use of these values in our print statement
      write(*,390) trim(hostname),  procid, numproc
 390  format("hello, fortran77 world; from host ", 
     %     A, ": process ", I3, " of ", I3)

C Finalise our execution environment and exit 
      call MPI_FINALIZE(ierr)

      end program
