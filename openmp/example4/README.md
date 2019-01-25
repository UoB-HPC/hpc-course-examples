Example 4: OpenMP Applications
========

In this directory, we continue to look at examples of work-sharing loops, first by considering the task of matrix multiplication and secondly by looking at the 1D heat equation.

Matrix Multiplication
----------------------

Matrix multiplication is often used as an example to highlight OpenMP work sharing in loops.
However, it is not really a sensible choice, as we will see in the next 3 example programs.

### Serial naive MM

This program contains some naive code to perform a matrix multiply.

### OMP naive MM

This program builds on the serial code and adds in OpenMP directives in an attempt to get a speed-up.
Timed, as ever, on my trusty 2-core laptop, I get a speed-up of ~1.5—not bad for a 2-core machine.
On BCp3, I see a speed-up on ~x6 when all 16 cores in a node are pressed into action for the OpenMP version.

### Serial BLAS MM

However, we see that the above approach isn't sensible in the real world, as we can perform a matrix multiplication using the highly optimised BLAS (Basic Linear Algebra Subprograms) library.
In this case, I can get close to the ~x6 speed-up we got by using 16 cores but this time I need only a single core—a much better use of resources!
This shows us that blindly following the parallelisation mantra isn't always sensible and that highly optimised serial code can beat naive parallel code.
This is especially attractive if someone else has already written the highly optimised code and made the library as ridiculously easy to install [as have the folks at TACC](http://www.tacc.utexas.edu/tacc-projects/gotoblas2).

Note that your mileage may vary, depending on which machine you are running on
and which implementation of the BLAS libraries you are using.
In fact, many BLAS implementations are multi-threaded themselves nowadays, so using such libraries can sometimes be the best route to gaining a parallel speed-up.

1D Heat Equation
----------------

### Serial heat

This is a nice short program which simulates heat diffusion along, say, a perfectly insulated iron bar.
The heat equation is common in science and engineering and so solving it quickly is useful.
Have a go at speeding it up using OpenMP.

(It turns out that this problem can also be profitably tackled using BLAS if we change the method of solution to an _implicit_ one, such as the Crank-Nicolson method; see https://source.ggy.bris.ac.uk/wiki/NumMethodsPDEs)
