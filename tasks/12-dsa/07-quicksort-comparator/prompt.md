# quicksort with a qsort-style comparator

Implement a generic in-place quicksort in `main.c`. Do **not** define
`main` — the grader links your file against a test harness that
provides it.

```c
#include <stddef.h>

/* Sort the array at base — nmemb elements, each size bytes wide —
 * into ascending order as defined by cmp. Same contract as the
 * standard qsort: cmp receives pointers to two ELEMENTS and returns
 * negative, zero, or positive for <, ==, >. */
void quick_sort(void *base, size_t nmemb, size_t size,
                int (*cmp)(const void *, const void *));
```

Rules and required behaviour:

- **You must implement the sort yourself.** Calling the library
  `qsort` fails the task: the harness links in a replacement `qsort`
  that prints a failure and exits. (Don't define your own function
  named `qsort` either — it will collide with that stub at link time.)
- Works for **any element size**: the harness sorts `int`s, structs,
  and `char *` string pointers (compared through `strcmp`). Move
  elements as raw bytes (`memcpy`/byte swaps); never assume a type.
- Whole elements travel together: a struct's other fields must stay
  attached to its key.
- When `nmemb` is 0 return immediately without touching `base`, which
  may be `NULL`. One element is already sorted. Duplicate and
  all-equal inputs must sort correctly (and must not loop forever or
  recurse without bound).
- Stability is **not** required.
- Performance matters: the harness sorts 100,000-element arrays that
  are **already sorted**, **reverse-sorted**, and pseudo-random. Pick
  your pivot so these all finish well inside the time limit — a
  first- or last-element pivot degrades to quadratic on sorted input
  and will time out. The middle element (or median-of-three) is fine.
- You may allocate a few `size`-byte scratch buffers (pivot copy, swap
  temp), but free them before returning: the harness runs leak-checked
  under AddressSanitizer/valgrind.

The harness verifies each result is ascending under the given
comparator and is an exact permutation of the input, then repeats
everything under AddressSanitizer/UBSan, so an off-by-one byte copy
fails the task even if the order looks right.

Grade with `ctrain check quicksort-comparator`.
