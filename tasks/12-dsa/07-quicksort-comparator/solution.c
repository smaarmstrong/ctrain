#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void swap_el(unsigned char *a, unsigned char *b, unsigned char *tmp,
                    size_t size)
{
    memcpy(tmp, a, size);
    memcpy(a, b, size);
    memcpy(b, tmp, size);
}

/* Hoare partition, middle pivot; recurse into the smaller half so the
 * stack stays O(log n) even on adversarial input. */
static void qs(unsigned char *a, long lo, long hi, size_t size,
               int (*cmp)(const void *, const void *),
               unsigned char *pivot, unsigned char *tmp)
{
    while (lo < hi) {
        memcpy(pivot, a + (size_t)(lo + (hi - lo) / 2) * size, size);
        long i = lo - 1, j = hi + 1;
        for (;;) {
            do
                i++;
            while (cmp(a + (size_t)i * size, pivot) < 0);
            do
                j--;
            while (cmp(a + (size_t)j * size, pivot) > 0);
            if (i >= j)
                break;
            swap_el(a + (size_t)i * size, a + (size_t)j * size, tmp, size);
        }
        if (j - lo < hi - (j + 1)) {
            qs(a, lo, j, size, cmp, pivot, tmp);
            lo = j + 1;
        } else {
            qs(a, j + 1, hi, size, cmp, pivot, tmp);
            hi = j;
        }
    }
}

void quick_sort(void *base, size_t nmemb, size_t size,
                int (*cmp)(const void *, const void *))
{
    if (nmemb < 2 || size == 0)
        return;
    unsigned char *scratch = malloc(2 * size);
    if (!scratch)
        return;
    qs(base, 0, (long)nmemb - 1, size, cmp, scratch, scratch + size);
    free(scratch);
}
