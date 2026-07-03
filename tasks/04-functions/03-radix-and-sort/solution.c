/* Reference solution. */
#include <stddef.h>

/* Writes the digits of n at *pos, most significant first, and
 * advances *pos past what it wrote. */
static void emit(unsigned long n, unsigned base, char *out, size_t *pos)
{
    if (n >= base)
        emit(n / base, base, out, pos);
    out[(*pos)++] = "0123456789abcdef"[n % base];
}

void fmt_base(unsigned long n, unsigned base, char *out)
{
    size_t pos = 0;
    emit(n, base, out, &pos);
    out[pos] = '\0';
}

static void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

/* Quicksort: middle element as pivot, Lomuto partition. */
void sort_ints(int *a, size_t n)
{
    if (n < 2)
        return;

    swap(&a[n / 2], &a[n - 1]); /* park the pivot at the end */
    int pivot = a[n - 1];
    size_t store = 0;
    for (size_t i = 0; i + 1 < n; i++)
        if (a[i] < pivot)
            swap(&a[i], &a[store++]);
    swap(&a[store], &a[n - 1]); /* pivot into its final slot */

    sort_ints(a, store);
    sort_ints(a + store + 1, n - store - 1);
}
