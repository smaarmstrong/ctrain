#include <stddef.h>

long find_first(const int *a, size_t n, int x)
{
    /* Invariant: every index < lo holds a value < x;
     * every index >= hi holds a value >= x. */
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (a[mid] < x)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < n && a[lo] == x)
        return (long)lo;
    return -1;
}
