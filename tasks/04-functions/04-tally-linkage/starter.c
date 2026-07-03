/* A tally module. Do NOT define main.
 *
 * The state below is static — internal linkage — so it cannot clash
 * with globals elsewhere in the program. Finish the module: min and
 * max are not tracked yet. */
#include <stddef.h>

static size_t n_values;
static long value_sum;
/* TODO: state for min and max */

void tally_reset(void)
{
    n_values = 0;
    value_sum = 0;
    /* TODO */
}

void tally_add(long v)
{
    n_values++;
    value_sum += v;
    /* TODO */
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
    return 0; /* TODO */
}

long tally_max(void)
{
    return 0; /* TODO */
}
