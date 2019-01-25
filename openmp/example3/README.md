Example 3: Work-sharing Loops
========

This example directory focuses on work-sharing loops, a mainstay of OpenMP programming.

Merging parallel sections
-------------------------

When a program contains several sections to be parallelised with OpenMP, you may be tempted to merge all of them within a single `parallel` region to avoid destroying and re-creating threads between sections.
However, virtually all recent OpenMP implementations will carefully manage the lifetime of their threads and will _not_ incur this overhead, so there is generally no reason to do it.

Have a look at the `regions-*.c` programs.
The `separate` version will enter and exit an OpenMP `parallel` region for every iteration of the outer loop, while the `merged` version has a single region.
Timing the two should give you identical results:

```bash
time ./regions-separate.exe
time ./regions-merged.exe
```


Work scheduling
---------------

### Simple

This program invokes the default work sharing algorithm.
The output from the program shows how the loop iterations are distributed between the cores.
On my laptop, I get two large contiguous sets of iterations executed by the two cores.
Compare this with the `static` scheduling clause.

On the cluster, you'll see different-sized contiguous blocks depending upon how many threads you invoke.
Most default implementations of OpenMP will use static as the default scheduling, but that's not guaranteed to be the case.

### Schedule

In this program, we change the work-sharing pattern and explicitly ask that the work be distributed in chunks of ten loop iterations.
Try experimenting with different scheduling instructions.

In this example, there is no work to do, so the different schedules do not significantly effect the run time.
However, scheduling patterns can be a good way to tune the load balance between the threads and so experimentation with scheduling should form part of an attempt to optimise your programs.

### Imbalance

In this program, we have deliberately introduced some (simulated) load imbalance.
Some of the loop iterations take much longer than others.

Experiment with different values for the schedule clause.
For example, you can compare the default schedule against a `dynamic` one, where idle threads will retrieve loop iterations from a queue of iterations to perform.
The default chunk size for dynamic is often 1.


Scheduling and NUMA
-------------------

Beware, however, as the task of load balancing can become rather intricate when we are running on NUMA architectures.
What is the balance between keeping all the processors busy vs. the delays associated with accessing memory attached to a remote socket on the motherboard?

Using the `numactl` command-line utility, we can specify which socket
(aka NUMA node) we would like to the memory to be allocated on, and which
socket we want the threads to run on.
We can ensure that these are the same socket, or force them to be different sockets.

For example, compare (calculate the average of several runs):

```bash
export OMP_NUM_THREADS=8
time numactl --membind=0 --cpunodebind=1 ./numa-bad.exe
time numactl --membind=1 --cpunodebind=1 ./numa-bad.exe
```

The operating system (kernel) is normally tasked with choosing the right NUMA settings and often does a good job.
For example, compare the timings for just running `time ./numa-bad.exe` to those above.
I get averages of ~2.9s, ~1.6s and ~1.8s respectively for these three permutations on a quiet, i.e. dedicated, cluster compute node.

The commonly used _first touch policy_ dictates that a memory page will be allocated into the memory on the same NUMA node as the processor running the process which first needs to _access_ that memory page.


The scheduling pattern of work sharing loops will interact with the first touch policy, affecting which NUMA node on the motherboard ends up holding data held in memory, e.g. portions a (perhaps 2d) array.
Explore this interaction as it could be useful for your assignment.

OpenMP 4.0 provides a clause to the `omp parallel` directive which can be used to control thread placement:

```C
#pragma omp parallel proc_bind(spread)
```

Valid arguments to the clause are `spread`, `close` and `master`.

Similar options can be controlled through the environment variables `OMP_PROC_BIND` and `OMP_PLACES`.
For example, the following command will place (and bind) threads as close to each-other as possible, filling the first socket before moving on to the second one:

```bash
env OMP_PLACES=cores OMP_PROC_BIND=close ./exe
```

You can get the OpenMP runtime to report all the relevant environment variables when it stars up if you set `OMP_DISPLAY_ENV=true`.
[This page from TACC](http://pages.tacc.utexas.edu/~eijkhout/pcse/html/omp-affinity.html) has more examples.

Placement Exercise
------------------

You may like to explore the use of thread placement options, perhaps in conjunction with the `numactl` utility.
You'll need a recent compiler version to use OpenMP 4.0; see the [SIMD section below](#simd) for more details of relevant modules on the cluster.

SIMD
----

OpenMP 4.0 includes the SIMD construct, e.g. (for C/C++):

```C
#pragma omp simd
```

This is intended as a portable instruction for the compiler to vectorize the contents of a loop.

You can try an example of this construct by typing `make simd`.
However, you will need a recent version of the relevant compiler in order to compile the code.

The example given in `simd.c` is contrived to provide problems for the auto-vectorizing abilities of a compiler and hence to show off the utility of the `simd` construct.

SIMD Exercise
--------

Try compiling the example code with the GNU and Intel compilers
listed below.
Try commenting out the `#pragma omp simd` construct, and recompiling.

What do you notice of the auto-vectorization of the loop?

- Intel compiler:
```bash
module add languages/intel-compiler-16-u2
```
- GNU compiler:
```bash
module add languages/gcc-7.1.0
# uncomment CFLAGS+=-fopt-info-vec-missed in the Makefile
```
