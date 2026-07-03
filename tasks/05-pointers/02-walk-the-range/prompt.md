# walk the range

A very common C idiom describes a slice of an array with two pointers:
`begin` points at the first element, `end` points **one past** the last.
The range `[begin, end)` is empty when `begin == end`.

Implement these three functions in `main.c`. Do **not** define `main` —
the grader links your file against a test harness that provides it.

```c
#include <stddef.h>

/* Sum of all ints in [begin, end). An empty range sums to 0. */
long sum_range(const int *begin, const int *end);

/* Pointer to the FIRST element in [begin, end) equal to x.
 * If no element matches (including the empty range), return end. */
const int *find_first(const int *begin, const int *end, int x);

/* How many elements in [begin, end) are strictly greater than x? */
size_t count_above(const int *begin, const int *end, int x);
```

Rules and edge cases:

- The range is always valid: `begin <= end`, and both may be equal.
- Never read at or past `end` — the grader re-runs everything under
  AddressSanitizer/UBSan, so an out-of-bounds read fails the task even
  if the returned values happen to look right.
- Whether you use pointer arithmetic (`p++`, `p < end`) or an index is
  up to you; behaviour is what is graded.

Grade with `ctrain check walk-the-range`.
