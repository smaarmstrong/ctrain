#include <stddef.h>

/* Insertion sort over raw bytes: rotate the current element back into
 * position with byte swaps, no scratch allocation needed. */

static void byte_swap(unsigned char *a, unsigned char *b, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        unsigned char t = a[i];
        a[i] = b[i];
        b[i] = t;
    }
}

void gsort(void *base, size_t n, size_t size,
           int (*cmp)(const void *, const void *))
{
    unsigned char *p = base;
    for (size_t i = 1; i < n; i++)
        for (size_t j = i; j > 0; j--) {
            unsigned char *cur = p + j * size;
            unsigned char *prev = cur - size;
            if (cmp(prev, cur) <= 0)
                break;
            byte_swap(prev, cur, size);
        }
}
