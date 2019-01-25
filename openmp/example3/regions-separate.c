#include <stdio.h>
#include <stdlib.h>

#define REPEAT 10
// #define N 67108864
#define N 536870912

int main()
{
    int *a = (int*)malloc(N*sizeof(int));
    int *b = (int*)malloc(N*sizeof(int));

    printf ("%d %d\n", a[1234],b[1234]);

    for (size_t j = 0; j < REPEAT; j++)
        #pragma omp parallel for
        for (size_t i = 0; i < N; i++)
            a[i] = b[i];

    printf ("%d %d\n", a[1234],b[1234]);

    return 0;
}
