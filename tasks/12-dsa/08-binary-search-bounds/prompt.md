# binary search for first and last occurrence

Implement two binary-search variants in `main.c`. Do **not** define
`main` — the grader links your file against a test harness that
provides it.

```c
#include <stddef.h>

/* a[0..n) is sorted ascending and may contain duplicates.
 * Return the index of the FIRST occurrence of key, or -1 if absent. */
long find_first(const int *a, size_t n, int key);

/* Same array contract.
 * Return the index of the LAST occurrence of key, or -1 if absent. */
long find_last(const int *a, size_t n, int key);
```

Required behaviour:

- The array is already sorted ascending; you never modify it.
- Duplicates matter: for the array `{1, 2, 2, 2, 3}`,
  `find_first(a, 5, 2)` is `1` and `find_last(a, 5, 2)` is `3`.
- A key that is not present — smaller than everything, larger than
  everything, or falling in a gap — returns `-1` from both functions.
- `n` may be 0, in which case `a` may be `NULL`; return `-1` without
  dereferencing it.
- **Both functions must run in O(log n).** The harness performs
  200,000 lookups against a 1,048,576-element array; a linear scan
  will blow the time limit and fail. It also repeats 100,000 lookups
  on a 1,048,576-element array that is one giant run of a single
  value — a plain binary search that finds *some* occurrence and then
  walks left/right is linear on that input and will also time out, so
  keep the whole search logarithmic.

The harness probes empty and single-element arrays, keys at both ends,
all-equal arrays, gaps between values, and the large-array timing
case, then repeats everything under AddressSanitizer/UBSan, so reading
`a[n]` or `a[-1]` fails the task even if the answers look right.

Grade with `ctrain check binary-search-bounds`.
