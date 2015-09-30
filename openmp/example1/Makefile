#
# Makefile to compile a selection of threaded programs
#

FORT_EXE=hello_f90.exe
C_EXE1=hello_c.exe 
C_EXE2=pthreads_hello.exe
EXES=$(FORT_EXE) $(C_EXE1) $(C_EXE2)

# compilers and flags
# by default we will choose GNU
COMP=GNU

ifeq ($(COMP), GNU)
  FC=gfortran
  CC=gcc
  OMP_FLAGS=-fopenmp
  PTHREAD_FLAGS=-pthread
endif

# settings for PGI compilers
ifeq ($(COMP), PGI)
  FC=pgf90
  CC=pgcc
  OMP_FLAGS=-mp
  PTHREAD_FLAGS=-lpthread
endif

# settings for Intel compilers
ifeq ($(COMP), INTEL)
  FC=ifort
  CC=icc
  OMP_FLAGS=-openmp
  PTHREAD_FLAGS=-pthread
endif

all: $(EXES)

$(FORT_EXE): %.exe : %.f90
	$(FC) $(OMP_FLAGS) $^ -o $@

$(C_EXE1): %.exe : %.c
	$(CC) $(OMP_FLAGS) $^ -o $@

$(C_EXE2): %.exe : %.c
	$(CC) $(PTHREAD_FLAGS) $^ -o $@

.PHONY: all clean spotless

clean:
	\rm -f $(EXES)

spotless: clean

