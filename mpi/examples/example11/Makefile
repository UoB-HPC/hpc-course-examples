#
# Makefile to build example MPI programs 
#

EXE1=skeleton1.exe
EXE2=skeleton2.exe
EXE3=skeleton3.exe
EXE4=skeleton4.exe
EXE5=skeleton2-simple2d.exe
EXE6=skeleton2-heated-plate.exe
EXE7=deadlock.exe
EXES=$(EXE1) $(EXE2) $(EXE3) $(EXE4) $(EXE5) $(EXE6) $(EXE7)

CFLAGS=-Wall -g -DDEBUG

all: $(EXES)

$(EXES): %.exe : %.c
	mpicc $(CFLAGS) -o $@ $^

.PHONY: clean all

clean:
	\rm -f $(EXES) 
	\rm -f *.o
