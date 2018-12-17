#
# Makefile to build example MPI programs
#

CC=mpicc

COMP=GNU

ifeq ($(COMP), GNU)
  CFLAGS=-Wall
  FFLAGS=-Wall
endif

EXES=hello_world_c


hello_world_c: hello_world.c
	$(CC) $(CFLAGS) -o $@ $^

all: $(EXES)

.PHONY: clean all

clean:
	\rm -f $(EXES)
	\rm -f *.o
