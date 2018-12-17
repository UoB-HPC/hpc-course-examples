Example 1: Hello, world
========

Welcome to the MPI programming examples!

In this directory we start, again, with "Hello, world".
MPI libraries can be used with several languages.
Here we have a program written in C, and [an advanced example](../advanced/example13/) has programs in C++, Fortran77 and Fortran90.

**Note**: Loading multiple MPI related modules could cause
unpredictable behaviour and runtime errors (as `mpirun` from
one module can end up being called with executables created
with compilers from another).

For simplicity, use the Intel compiler and MPI with these examples:

```bash
module add languages/intel-compiler-16-u2
```

You will then:

- Use `mpiicc` to invoke the Intel compiler, linking with the Intel (MPICH-based) MPI library.
- Use `mpirun` to launch your application.

