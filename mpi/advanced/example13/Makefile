#
# Makefile to build example MPI programs
#

CXX=mpiCC
FC=mpif77
F90=mpif90

COMP=GNU

ifeq ($(COMP), GNU)
  CFLAGS=-Wall
  FFLAGS=-Wall
endif

EXES=hello_world_f77 hello_world_f90 hello_world_c++

hello_world_c++: hello_world.cc
	$(CXX) $(CFLAGS) -o $@ $^
hello_world_f77: hello_world.f
	$(FC) $(FFLAGS) -o $@ $^

hello_world_f90: hello_world.f90
	$(F90) $(FFLAGS) -o $@ $^

all: $(EXES)

.PHONY: clean all

clean:
	\rm -f $(EXES)
	\rm -f *.o
