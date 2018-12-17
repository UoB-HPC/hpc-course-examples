Example 12: Cartesian Communicators and Neighbourhood Collective Functions
==========================================================================

cartestian.c:
-------------

An example of using an MPI communicator with knowledge of a cartesian grid.
The program uses some helper function to discover the coordinates of the rank
in the grid as well as which other ranks are the grid neighbours.

MPI_neighbor_alltoall:
----------------------

An example of calling a neighbourhood collective function.

**Note**: neighbourhood collectives were introduced in MPI-3 and you will need a 
compiler which supports MPI-3 in order to compile the program, e.g. on BC3:
- OpenMPI v1.6.5 – will _not_ work.
- `languages/intel-compiler-16-u2` – will work.

The call to `neighbor_alltoall()` simultaneously sends and receives data to/from
the neighbouring ranks in the cartesian grid.  Handy!

