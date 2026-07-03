#include <stddef.h>
#include <stdlib.h>

typedef struct {
    int key;   /* the ONLY field the sort order depends on */
    int tag;   /* payload — must travel with its key, never compared */
} Rec;

void insertion_sort(Rec *a, size_t n)
{
    /* TODO: sort a[0..n) ascending by key, stably, in place. */
    (void)a;
    (void)n;
}

void merge_sort(Rec *a, size_t n)
{
    /* TODO: stable O(n log n) mergesort; free any scratch you allocate. */
    (void)a;
    (void)n;
}
