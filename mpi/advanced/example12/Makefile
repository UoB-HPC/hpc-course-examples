#
# Makefile to build example MPI programs 
#

EXE1=cartesian.exe
EXE2=neighbor_alltoall.exe
EXES=$(EXE1) $(EXE2)

CFLAGS=-Wall -g -DDEBUG

all: $(EXES)

$(EXES): %.exe : %.c
	mpiicc $(CFLAGS) -o $@ $^

.PHONY: clean all

clean:
	\rm -f $(EXES) 
	\rm -f *.o
