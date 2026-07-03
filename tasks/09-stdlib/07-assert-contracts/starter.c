#include <assert.h>
#include <ctype.h>
#include <stdlib.h>

double clamped(double x, double lo, double hi)
{
    /* TODO: assert the precondition lo <= hi, then clamp x into [lo, hi].
     * The check must abort in the normal build and vanish under NDEBUG. */
    (void)lo;
    (void)hi;
    return x;
}

int parse_port(const char *s)
{
    /* TODO: runtime validation — never assert here. Digits only,
     * 1..65535, -1 for everything else. Watch for overflow on long
     * digit strings. */
    (void)s;
    return 0;
}
