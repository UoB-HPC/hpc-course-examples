#
# Makefile to build example MPI programs 
#

CC=mpiicc

CFLAGS=-Wall

EXE1=broadcast.exe
EXE2=reduce_trapezoid.exe
EXE3=scatter_gather.exe
EXES=$(EXE1) $(EXE2) $(EXE3)

all: $(EXES)

$(EXES): %.exe : %.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean all

clean:
	\rm -f $(EXES) 
	\rm -f *.o
