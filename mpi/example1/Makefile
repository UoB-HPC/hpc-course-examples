#
# Makefile to build example MPI programs
#

CC=mpiicc

CFLAGS=-Wall

EXES=hello_world_c.exe


hello_world_c.exe: hello_world.c
	$(CC) $(CFLAGS) -o $@ $^

all: $(EXES)

.PHONY: clean all

clean:
	\rm -f $(EXES)
	\rm -f *.o
