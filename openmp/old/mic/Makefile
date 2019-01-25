#
# Makefile to compile a selection of OpenMP 4.0 programs
#

EXE1=query.exe 
EXE2=saxpy-serial.exe 
EXE3=saxpy-mic.exe 

EXES=$(EXE1) $(EXE2) $(EXE3)

# compilers and flags
# by default we will choose GNU
COMP=INTEL

ifeq ($(COMP), GNU)
  CC=gcc
  OMP_FLAGS=-fopenmp
  PTHREAD_FLAGS=-pthread
endif

ifeq ($(COMP), PGI)
  CC=pgcc
  OMP_FLAGS=-mp
  PTHREAD_FLAGS=-lpthread
endif

# settings for Intel compilers
ifeq ($(COMP), INTEL)
  CC=icc
  OMP_FLAGS=-openmp
  PTHREAD_FLAGS=-pthread
endif

all: $(EXES)

$(EXES): %.exe : %.c
	$(CC) $(OMP_FLAGS) $^ -o $@

.PHONY: all clean spotless

clean:
	\rm -f $(EXES)

spotless: clean

