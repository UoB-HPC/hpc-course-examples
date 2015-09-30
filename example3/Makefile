#
#
EXE1=simple.exe
EXE2=schedule.exe
EXE3=imbalance.exe
EXE4=numa-bad.exe
EXE5=simd.exe
EXES=$(EXE1) $(EXE2) $(EXE3) $(EXE4)

# compilers and flags
# by default we will choose GNU
COMP=GNU

ifeq ($(COMP), GNU)
  CC=gcc
  CFLAGS=-g -O3 -fopenmp -lm
#  CFLAGS+=-fopt-info-vec-missed
endif

# settings for PGI compilers
ifeq ($(COMP), PGI)
  CC=pgcc
  CFLAGS=-g -fast -mp
endif

# settings for Intel compilers
ifeq ($(COMP), INTEL)
  CC=icc
  CFLAGS=-g -O3 -openmp
  CFLAGS+=-qopt-report-phase=vec
endif

all: $(EXES)

# NB, you'll need a recent compiler version to compile the SIMD example
# e.g.
# - for GNU:   languages/gcc-5.0 
# - for Intel: languages/intel-compiler-15
simd: $(EXE5)

$(EXE5): simd.c
	$(CC) $(CFLAGS) $^ -o $@

$(EXES): %.exe : %.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: all clean spotless simd

clean:
	\rm -f $(EXES) $(EXE5)

spotless: clean
