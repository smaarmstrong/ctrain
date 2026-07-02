#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

void reverse(int *a, size_t n)
{
    for (size_t i = 0; n > 1 && i < n - 1 - i; i++)
        swap(&a[i], &a[n - 1 - i]);
}

int *clone_ints(const int *src, size_t n)
{
    if (n == 0)
        return NULL;
    int *dup = malloc(n * sizeof *dup);
    if (!dup)
        return NULL;
    memcpy(dup, src, n * sizeof *dup);
    return dup;
}
