#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int key;
    int tag;
} Rec;

void insertion_sort(Rec *a, size_t n)
{
    for (size_t i = 1; i < n; i++) {
        Rec v = a[i];
        size_t j = i;
        /* strict > keeps equal keys in input order: stable */
        while (j > 0 && a[j - 1].key > v.key) {
            a[j] = a[j - 1];
            j--;
        }
        a[j] = v;
    }
}

static void msort(Rec *a, Rec *tmp, size_t lo, size_t hi)
{
    if (hi - lo < 2)
        return;
    size_t mid = lo + (hi - lo) / 2;
    msort(a, tmp, lo, mid);
    msort(a, tmp, mid, hi);

    size_t i = lo, j = mid, k = lo;
    while (i < mid && j < hi) {
        /* take from the left run on ties: stable */
        if (a[j].key < a[i].key)
            tmp[k++] = a[j++];
        else
            tmp[k++] = a[i++];
    }
    while (i < mid)
        tmp[k++] = a[i++];
    while (j < hi)
        tmp[k++] = a[j++];
    memcpy(a + lo, tmp + lo, (hi - lo) * sizeof *a);
}

void merge_sort(Rec *a, size_t n)
{
    if (n < 2)
        return;
    Rec *tmp = malloc(n * sizeof *tmp);
    if (!tmp)
        return;
    msort(a, tmp, 0, n);
    free(tmp);
}
