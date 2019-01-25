Example 6: Debugging
========

Simple
------

In this program we see a common mishap when using a work-sharing construct around a nested loop.
The loop indices for the nested loops need to be private to the thread.
If this is not the case, the inner loops _can_ modify the indices out of sequence, and so we can end up inadvertently skipping iterations.

Capricious
----------

1. Type `make` to compile the code and run `capricious.exe` a few times.
   How often does it run OK and how often does it halt with an error?
2. Try varying the size of the array.
   What is the effect of a smaller array? How about a bigger one?
3. Edit the Makefile so that the `-O3` flag is added to the compiler flags.
   What effect does this have?
4. Edit the Makefile again so that `-DPRINT` is added to the compiler flags.
   Does this alter the relative frequency of pass or fail?
5. Changing `NUM_THREADS` to `1` in the source code will obviously eradicate the race condition, right?
6. If you have Valgrind available to you, try to debug the program by running:
   ```bash
   valgrind --tool=drd ./capricious.exe
   ```
   Does this help us?
7. We can also try running the program through the GNU debugger:
   ```bash
   gdb ./capricious.exe
   ```
   We can:
     - put a watch on `ii`
     - add breakpoints
     - step through the program
     - switch from thread to thread

  What benefits does any of this bring?

