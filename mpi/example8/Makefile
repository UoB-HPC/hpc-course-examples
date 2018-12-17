#
# Makefile to build example MPI programs 
#

CC=mpicc

COMP=GNU
ifeq ($(COMP), GNU)
  CFLAGS=-Wall
endif

EXE1=rma_trapezoid.exe
EXES=$(EXE1)

all: $(EXES)

$(EXES): %.exe : %.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean all

clean:
	\rm -f $(EXES) 
	\rm -f *.o
