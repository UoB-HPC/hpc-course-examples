Example 1: Hello, world
======================

We begin our OpenMP examples with—of course—the quintessential "hello, world" program.

OpenMP is available to C, C++, and Fortran programmers.
C and Fortran90 versions are included in this directory.
These can be contrasted with a similar program written using POSIX threads directly.

## Building

To build the examples, the GNU compiler will be used by default, so you should load the module with the latest version:

    module load languages/gcc-7.1.0

Then, simply type `make` to create the executables.

If you want to use a different compiler, you will need to set the `COMP` variable.
Have a look inside the `Makefile` for the possible options.
For example, to build with the Intel compiler:

    module load languages/intel-compiler-16-u2
    make COMP=INTEL

There is also a submission script included for use on Blue Crystal, which is called `openmp_submit`.
You can submit a job to the cluster's queue using `qsub`:

    qsub openmp_submit
