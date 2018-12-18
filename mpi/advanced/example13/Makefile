#
# Makefile to build example MPI programs
#

CXX=mpiicpc
FC=mpif77
F90=mpif90

CFLAGS=-Wall
FFLAGS=-Wall

EXES=hello_world_f77.exe hello_world_f90.exe hello_world_c++.exe

hello_world_c++.exe: hello_world.cc
	$(CXX) $(CFLAGS) -o $@ $^
hello_world_f77.exe: hello_world.f
	$(FC) $(FFLAGS) -o $@ $^

hello_world_f90.exe: hello_world.f90
	$(F90) $(FFLAGS) -o $@ $^

all: $(EXES)

.PHONY: clean all

clean:
	\rm -f $(EXES)
	\rm -f *.o
