#include "stats.h"

struct stats stats_of(const long *values, size_t n)
{
    struct stats s = {.count = n};

    if (n == 0)
        return s;

    s.min = values[0];
    s.max = values[0];
    for (size_t i = 0; i < n; i++) {
        if (values[i] < s.min)
            s.min = values[i];
        if (values[i] > s.max)
            s.max = values[i];
        s.sum += values[i];
    }
    s.mean = (double)s.sum / (double)n;
    return s;
}
