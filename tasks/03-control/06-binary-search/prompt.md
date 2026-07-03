# Binary search: first occurrence in a sorted array

Implement this function in `main.c`. Do **not** define `main` — the
grader links your file against a test harness that provides it.

```c
#include <stddef.h>

/* a points to n ints sorted in ascending order; duplicates may occur.
 * Return the SMALLEST index i with a[i] == x, or -1 if x is absent.
 * n may be 0, and a may be NULL when n is 0.
 * Never read outside a[0] .. a[n-1]. */
long find_first(const int *a, size_t n, int x);
```

Requirements:

- **First occurrence.** In `{1, 3, 3, 3, 5}` searching for `3` must
  return `1`, not `2` or `3`. In `{7, 7, 7}` searching for `7` returns
  `0`. A textbook search that returns as soon as it sees a match will
  fail here — keep narrowing until only one candidate is left.
- **All the misses.** Return `-1` for a target smaller than everything,
  larger than everything, in a gap between elements, and for `n == 0`.
- **Stay inside the array.** The harness allocates each test array at
  exactly `n * sizeof(int)` and re-runs everything under
  AddressSanitizer/UBSan: touching `a[n]`, `a[-1]`, or dereferencing
  the NULL empty-array pointer fails the task even when the returned
  indices are right.
- **Logarithmic, not linear.** Halve the search range each step —
  compute the midpoint as `lo + (hi - lo) / 2` so it cannot overflow.
  (The grader cannot see your loop, but a linear scan defeats the
  point of the exercise.)
- Mind `size_t`: it is unsigned, so expressions like `hi - 1` when
  `hi` is `0` wrap around to a huge value. Choose bounds that never
  need to go below zero — a half-open `[lo, hi)` range works well.

Grade with `ctrain check binary-search`.
