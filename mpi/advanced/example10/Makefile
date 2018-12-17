#
# Makefile to build example MPI programs 
#

CC=mpicc

COMP=GNU
ifeq ($(COMP), GNU)
  CFLAGS=-Wall
endif


EXE1=group_to_comm.exe
EXE2=split.exe
EXES=$(EXE1) $(EXE2) 

all: $(EXES)

$(EXES): %.exe : %.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean all

clean:
	\rm -f $(EXES) 
	\rm -f *.o
