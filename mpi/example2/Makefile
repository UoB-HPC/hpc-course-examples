#
# Makefile to build example MPI programs
#

CC=mpiicc

CFLAGS=-Wall

EXE1=send_recv.exe
EXE2=deadlock.exe
EXE3=simple-pingpong.exe
EXES=$(EXE1) $(EXE2) $(EXE3)

all: $(EXES)

$(EXES): %.exe : %.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean all

clean:
	\rm -f $(EXES)
	\rm -f *.o
