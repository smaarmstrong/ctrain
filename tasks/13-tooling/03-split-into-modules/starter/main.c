/* statcalc — everything in one file. Split me into
 * stats.h / stats.c / main.c and write a Makefile with real
 * dependencies. The printed line must not change. See prompt.md.
 */
#include <stddef.h>
#include <stdio.h>

static const int readings[] = {12, 997, -5, 300, 41, 88, 512, 525};
#define NREADINGS (sizeof readings / sizeof readings[0])

static long sum_i(const int *a, size_t n)
{
    long s = 0;
    for (size_t i = 0; i < n; i++)
        s += a[i];
    return s;
}

static int max_i(const int *a, size_t n)
{
    int m = a[0];
    for (size_t i = 1; i < n; i++)
        if (a[i] > m)
            m = a[i];
    return m;
}

static double mean_i(const int *a, size_t n)
{
    return (double)sum_i(a, n) / (double)n;
}

int main(void)
{
    printf("n=%zu sum=%ld max=%d mean=%.2f\n",
           NREADINGS,
           sum_i(readings, NREADINGS),
           max_i(readings, NREADINGS),
           mean_i(readings, NREADINGS));
    return 0;
}
