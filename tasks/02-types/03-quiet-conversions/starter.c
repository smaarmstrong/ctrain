#include <stddef.h>

double mean_of(const int *a, size_t n)
{
    /* TODO: sum in a type wide enough, divide in floating point. */
    (void)a;
    (void)n;
    return 0.0;
}

int percent_of(int part, int whole)
{
    /* TODO: (part * 100) / whole without the multiplication
     * overflowing int. */
    (void)part;
    (void)whole;
    return 0;
}

unsigned char low_byte(long v)
{
    /* TODO: low 8 bits, well defined for negative v. */
    (void)v;
    return 0;
}

int clamp_to_int(double d)
{
    /* TODO: truncate in range, saturate outside it — and never cast
     * an out-of-range double to int. */
    (void)d;
    return 0;
}
