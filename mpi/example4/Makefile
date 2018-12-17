#
# Makefile to build example MPI programs 
#

CC=mpicc

COMP=GNU
ifeq ($(COMP), GNU)
  CFLAGS=-Wall
endif

EXE1=blocking.exe
EXE2=non-blocking.exe
EXE3=bugged-non-blocking.exe
EXES=$(EXE1) $(EXE2) $(EXE3) 

all: $(EXES)

$(EXES): %.exe : %.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean all

clean:
	\rm -f $(EXES) 
	\rm -f *.o
