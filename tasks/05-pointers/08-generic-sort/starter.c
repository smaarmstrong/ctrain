#include <stddef.h>

/* The array is just bytes: element i lives at
 *     (unsigned char *)base + i * size
 * Compare two elements ONLY with cmp(p, q); move them with memcpy /
 * memmove / a byte-swap loop over `size` bytes. */

void gsort(void *base, size_t n, size_t size,
           int (*cmp)(const void *, const void *))
{
    /* TODO: any comparison sort you like — insertion sort is plenty. */
    (void)base;
    (void)n;
    (void)size;
    (void)cmp;
}
