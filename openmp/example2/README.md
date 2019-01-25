Example 2: Common Pitfalls
==========================

In this directory, we encounter several programs to estimate the value of pi.
We will see an evolution from serial code to OpenMP parallel code, highlighting the common pitfalls of _cache thrashing_ and _false sharing_ along the way.

Serial pi
---------

- Try varying the value of num_steps and see the effect upon run-time and the magnitude in the error in estimating pi.
- The `time` command is a useful tool for rough-and-ready timings, e.g.:
    ```
    time ./serial_pi.exe
    ```

Parallel shared pi
------------------

Here we see what happens if we share an accumulator over multiple threads.

Running `time ./parallel_shared_pi.exe` reveals the full horror of the cache thrashing and the pain associated with acquiring and releasing all the locks in the critical region (mutex).
Try experimenting with the number of threads used.
The problem just gets worse as we move from 4 to 8 to 16 threads.

Parallel array pi
-----------------

This program is an attempt to correct the cache thrashing but falls foul of the pitfall of _false sharing_, as although the different threads use different cells of the array as accumulators, the whole array is small and can end up in cache and so coherency must be maintained.
On a two-core laptop, this program is still slower than the serial code.
See how it goes for you on the cluster.

Parallel private pi
-------------------

In this program we have finally corrected our mistakes and have declared an accumulator that is private to each thread.
We are rewarded by a corresponding reduction in the overall runtime.
A speed-up of just shy of 2 on my Centrino duo laptop.
Again, see what is possible on a node of the cluster.

A good exercise is to experiment with different values of the
`OMP_NUM_THREADS` environment variable and to plot run-times against the number
of threads used.
The result may surprise you.
If it does, think about what work is available to do and how many threads that can profitably be shared
amongst.
What are the overheads in adding more threads to the calculation?

**Note**: Be sure to adjust the resource request in your submission script to match
any changes you make to `OMP_NUM_THREADS`—if you want your threads to have
their own processor core.

Reduction pi
------------

Last but not least is a version which uses a _reduction_.
This approach is the most elegant and offers the best potential for scaling the accumulator part of the code over a larger number of cores.
Compare the run-times for this approach with those for the previous program.

Starting to profile your code
-----------------------------

Since these programs are very small, the rough timings and differences in the code are sufficient to tell us what is going on.
However, as your code gets more complex, you will want to avail yourself of wonderful world of profilers.

We have several installed on BCp3 and you should start by looking at Intel's VTune and the open source TAU.
