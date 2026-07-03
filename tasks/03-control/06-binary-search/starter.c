#include <stddef.h>

/* A classic binary search — but it returns WHICHEVER occurrence it
 * happens to hit, not the smallest index, so it fails on duplicates.
 * Fix it while keeping it O(log n). */
long find_first(const int *a, size_t n, int x)
{
    size_t lo = 0, hi = n; /* half-open: candidates are [lo, hi) */
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (a[mid] == x)
            return (long)mid; /* BUG: an earlier equal element may exist */
        if (a[mid] < x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return -1;
}
