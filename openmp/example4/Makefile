
EXE1=serial_naive_mm.exe
EXE2=omp_naive_mm.exe
EXE3=serial_blas_mm.exe
EXE4=serial_heat.exe

EXES=$(EXE1) $(EXE2) $(EXE3) $(EXE4)

CC=gcc
CFLAGS=-O3
# for bluecrystalp3:
BLASLINK=-L/cm/shared/apps/gotoblas/penryn/64/1.26 -lgoto -Wl,-rpath,/cm/shared/apps/gotoblas/penryn/64/1.26
# for bluecrystalp2:
#BLASLINK=-L/usr/local/gnu_builds/ATLAS/lib -lf77blas -latlas -Wl,-rpath,/usr/local/gnu_builds/ATLAS/lib
# for bluecrystalp1:
#BLASLINK=-L/usr/local/libraries/gcc-4.6.2-build/ATLAS/lib -lf77blas -latlas -lgfortran -Wl,-rpath,/usr/local/libraries/gcc-4.6.2-build/ATLAS/lib

all: $(EXES)

$(EXE1): %.exe : %.c
	$(CC) $(CFLAGS) $^ -o $@

$(EXE2): %.exe : %.c
	$(CC) $(CFLAGS) -fopenmp $^ -o $@

$(EXE3): %.exe : %.c
	$(CC) $(CFLAGS) $^ $(BLASLINK) -o $@ 

$(EXE4): %.exe : %.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: all clean

clean:
	\rm -f $(OBJS) $(EXES)
