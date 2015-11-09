#
# Makefile to build example MPI programs 
#

EXE1=spawn.exe
EXE2=
EXES=$(EXE1) $(EXE2) 

all: $(EXES)

$(EXES): %.exe : %.c
	mpicc -o $@ $^

.PHONY: clean all

clean:
	\rm -f $(EXES) 
	\rm -f *.o
