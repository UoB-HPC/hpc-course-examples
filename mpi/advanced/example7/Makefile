#
# Makefile to build example MPI programs 
#

CC=mpiicc

CFLAGS=-Wall

EXE1=pack-unpack.exe
EXE2=derived-type.exe
EXES=$(EXE1) $(EXE2)

all: $(EXES)

$(EXES): %.exe : %.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean all

clean:
	\rm -f $(EXES) 
	\rm -f *.o
