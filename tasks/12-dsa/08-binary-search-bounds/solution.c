#include <stddef.h>

/* lower_bound: first index whose element is >= key */
long find_first(const int *a, size_t n, int key)
{
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (a[mid] < key)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < n && a[lo] == key)
        return (long)lo;
    return -1;
}

/* upper_bound minus one: last index whose element is <= key */
long find_last(const int *a, size_t n, int key)
{
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (a[mid] <= key)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo > 0 && a[lo - 1] == key)
        return (long)(lo - 1);
    return -1;
}
