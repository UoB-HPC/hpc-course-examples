#
#
EXE1=quicksort.exe
EXE2=quicksort_sections.exe

EXES=$(EXE1) $(EXE2)

# compilers and flags
# by default we will choose GNU
COMP=GNU

ifeq ($(COMP), GNU)
  FC=gfortran
  CC=gcc
  FFLAGS=-fopenmp
  CFLAGS=-fopenmp
endif

# settings for PGI compilers
ifeq ($(COMP), PGI)
  FC=pgf90
  CC=pgcc
  FFLAGS=-mp
  CFLAGS=-mp
endif

# settings for Intel compilers
ifeq ($(COMP), INTEL)
  FC=ifort
  CC=icc
  FFLAGS=-openmp
  CFLAGS=-openmp
endif

all: $(EXES)

$(EXES): %.exe : %.c
	$(CC) $(CFLAGS) -O3 $^ -o $@

.PHONY: all clean

clean:
	\rm -f $(EXES)
