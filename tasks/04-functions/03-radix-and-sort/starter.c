/* Implement fmt_base and sort_ints. Do NOT define main. */
#include <stddef.h>

void fmt_base(unsigned long n, unsigned base, char *out)
{
    /* TODO: recurse on n / base before emitting n % base.
     * Hint: a small static or helper-managed index, or a helper
     * that returns the write position, keeps this tidy. */
    (void)n;
    (void)base;
    out[0] = '0';
    out[1] = '\0';
}

void sort_ints(int *a, size_t n)
{
    /* TODO: partition around a pivot, recurse on both halves. */
    (void)a;
    (void)n;
}
