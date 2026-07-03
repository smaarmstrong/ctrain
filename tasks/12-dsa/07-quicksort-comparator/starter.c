#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void quick_sort(void *base, size_t nmemb, size_t size,
                int (*cmp)(const void *, const void *))
{
    /* TODO: in-place quicksort over raw bytes.
     * Hints: treat base as unsigned char *, element i lives at
     * base + i * size; swap elements with memcpy through a temp;
     * pick a middle pivot so sorted input stays O(n log n). */
    (void)base;
    (void)nmemb;
    (void)size;
    (void)cmp;
}
