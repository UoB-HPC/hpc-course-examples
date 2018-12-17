Example 8: MPI File I/O
=======================

This directory concerns file input and output.

MPI-1 did not contain any functions for file I/O.  However,
MPI-2 contains a number of functions which accommodate
parallel access to files.

file-io
-------

In this program we arrange for all of the processors in a
communicator to write to the same file.  They are each
assigned a window—known as a file _view_—within which
they can operate.  I have set the window to be 10 characters
wide and to be offset from the start of the file by 10x the
rank of the process, in bytes.

### Exercises

Inspect the contents of `myfile` as you vary the processor
count.

- You might like to write a program to read from a single file
  in parallel.  This can be a convenient way to initialise your
  program.

- Modify the program to use non-blocking read and write functions:
  `MPI_File_iread()`
  `MPI_File_iwrite()`
  and devise a way to contrast these to calls to their blocking equivalents.

- Experiment with the use of shared file pointers so that each process can
  read the same data from the same file:
  `MPI_File_read_shared()`.
