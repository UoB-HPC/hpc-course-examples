#
#

EXE1=simple.exe
EXE2=capricious.exe
EXES=$(EXE2) $(EXE1)

# compilers and flags
# by default we will choose GNU
COMP=GNU

ifeq ($(COMP), GNU)
  CC=gcc
  CFLAGS=-g -fopenmp -Wall
  LDFLAGS=-lm
#  CFLAGS+=-O3
#  CFLAGS+=-DPRINT
#  CFLAGS+=-DDEBUG
endif

# settings for PGI compilers
ifeq ($(COMP), PGI)
  CC=pgcc
  CFLAGS=-mp
endif

# settings for Intel compilers
ifeq ($(COMP), INTEL)
  CC=icc
  CFLAGS=-openmp
endif

all: $(EXES)

$(EXES): %.exe : %.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

.PHONY: all clean

clean:
	\rm -f $(OBJS) $(EXES)
