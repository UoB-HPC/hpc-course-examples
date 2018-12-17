#
# Makefile to build example MPI programs 
#

CC=mpicc

COMP=GNU
ifeq ($(COMP), GNU)
  CFLAGS=-Wall
endif

EXE1=individual-file.exe
EXE2=MPI-file-view.exe
EXE3=MPI-file-iwrite.exe
EXE4=collated.exe
EXES=$(EXE1) $(EXE2) $(EXE3) $(EXE4)

all: $(EXES)

$(EXES): %.exe : %.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean all

clean:
	\rm -f $(EXES) 
	\rm -f *.txt
