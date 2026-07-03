#include <assert.h>
#include <ctype.h>
#include <stddef.h>

double clamped(double x, double lo, double hi)
{
    assert(lo <= hi); /* caller bug if not — gone under NDEBUG */

    if (x < lo)
        return lo;
    if (x > hi)
        return hi;
    return x;
}

int parse_port(const char *s)
{
    if (s == NULL || *s == '\0')
        return -1;

    long v = 0;
    for (; *s != '\0'; s++) {
        if (!isdigit((unsigned char)*s))
            return -1;
        v = v * 10 + (*s - '0');
        if (v > 65535)
            return -1; /* also stops v from ever overflowing */
    }
    return v >= 1 ? (int)v : -1;
}
