#include <stddef.h>

long sum_range(const int *begin, const int *end)
{
    long total = 0;
    for (const int *p = begin; p < end; p++)
        total += *p;
    return total;
}

const int *find_first(const int *begin, const int *end, int x)
{
    for (const int *p = begin; p < end; p++)
        if (*p == x)
            return p;
    return end;
}

size_t count_above(const int *begin, const int *end, int x)
{
    size_t n = 0;
    for (const int *p = begin; p < end; p++)
        if (*p > x)
            n++;
    return n;
}
