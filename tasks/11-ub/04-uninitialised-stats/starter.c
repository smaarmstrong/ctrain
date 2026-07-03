#include <stddef.h>

/* Two helpers, two variables read before anything was stored in them.
 * Find the missing initialisations. */

int max_of(const int *a, size_t n)
{
    int best;
    for (size_t i = 1; i < n; i++)
        if (a[i] > best)
            best = a[i];
    return best;
}

long long sum_of(const int *a, size_t n)
{
    long long sum;
    for (size_t i = 0; i < n; i++)
        sum += a[i];
    return sum;
}
