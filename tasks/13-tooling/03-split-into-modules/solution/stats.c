#include "stats.h"

long sum_i(const int *a, size_t n)
{
    long s = 0;
    for (size_t i = 0; i < n; i++)
        s += a[i];
    return s;
}

int max_i(const int *a, size_t n)
{
    int m = a[0];
    for (size_t i = 1; i < n; i++)
        if (a[i] > m)
            m = a[i];
    return m;
}

double mean_i(const int *a, size_t n)
{
    return (double)sum_i(a, n) / (double)n;
}
