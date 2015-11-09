/*
** An example of using the trapezoidal rule to numerically 
** integrate f(x) from the points a to b.
** The rule boils down to the summation:
** [f(a)/2 +f(b)/2 + f(x1) + f(x2) + ... + f(xN)] * w
** where w is the width of each trapezoid, which is in turn
** determined by the number (N) of trapezoids we use in 
** the estimation.
** In this case we've hardwired a, b, N, and f(x).
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define sqr(x)((x)*(x))
#define N 15

double f(double x);  /* the function that we are integrating */

int main() {

  double integral;  /* where we'll store the result of the integration */
  double a = 0.0;   /* left endpoint of definite integration */
  double b = 1.0;   /* right endpoint of definite integration */
  double width;     /* width of each trapezoid */
  double x;         /* point in the range [a,b] */

  double PI25DT = 3.141592653589793238462643;

  int ii;

  width = (b - a)/(double)N;

  /* the first two elements in our series */
  integral = (f(a) + f(b))/2.0;

  /* loop over the rest */ 
  x = a;
  for (ii=1; ii<N; ii++) {
    x += width;
    integral += f(x);
  }
  /* don't forget the final multiplier */
  integral *= width;

  printf("Definite integral from %f to %f estimated as %f (using %d trapezoids)\n",
	 a, b, integral, N);
  printf("(error: %.16f)\n",fabs(integral - PI25DT));

  return EXIT_SUCCESS;
}

/* definite integral of this will estimate pi */
double f(double x) {
  return 4.0 / (1 + sqr(x));
}
