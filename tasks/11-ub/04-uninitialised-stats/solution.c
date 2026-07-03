#include <stddef.h>

int max_of(const int *a, size_t n)
{
    int best = a[0];
    for (size_t i = 1; i < n; i++)
        if (a[i] > best)
            best = a[i];
    return best;
}

long long sum_of(const int *a, size_t n)
{
    long long sum = 0;
    for (size_t i = 0; i < n; i++)
        sum += a[i];
    return sum;
}
