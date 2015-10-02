#
# Makefile to build example OpenACC programs 
#

CC=pgcc
CFLAGS=-acc -fast -Minfo

EXE1=saxpy.exe
EXES=$(EXE1)

all: $(EXES)

$(EXES): %.exe : %.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean all

clean:
	\rm -f $(EXES) 
	\rm -f *.o
