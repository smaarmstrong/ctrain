/* Reference solution. All state is static: internal linkage. */
#include <stddef.h>

static size_t n_values;
static long value_sum;
static long value_min;
static long value_max;

void tally_reset(void)
{
    n_values = 0;
    value_sum = 0;
    value_min = 0;
    value_max = 0;
}

void tally_add(long v)
{
    if (n_values == 0) {
        value_min = v;
        value_max = v;
    } else {
        if (v < value_min)
            value_min = v;
        if (v > value_max)
            value_max = v;
    }
    n_values++;
    value_sum += v;
}

size_t tally_count(void)
{
    return n_values;
}

long tally_sum(void)
{
    return value_sum;
}

long tally_min(void)
{
    return value_min;
}

long tally_max(void)
{
    return value_max;
}
