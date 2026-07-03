# recursion: radix formatter and in-place sort

Implement the two functions below in `main.c`. Do **not** define
`main` — the grader links your file against a test harness that
provides it.

```c
#include <stddef.h>

/* Write n in the given base into out as a NUL-terminated string,
 * most significant digit first. base is always in 2..16; use the
 * lowercase letters 'a'..'f' for digit values 10..15.
 * fmt_base(0, b, out) must produce "0".
 * out is always large enough (at least 65 bytes). */
void fmt_base(unsigned long n, unsigned base, char *out);

/* Sort the first n ints of a into ascending order, in place.
 * n may be 0, in which case a may be NULL. Duplicates allowed. */
void sort_ints(int *a, size_t n);
```

Both are natural fits for recursion, and that is the point of this
exercise:

- `fmt_base`: the awkward part is that arithmetic peels digits off
  *least*-significant-first while the output wants them
  *most*-significant-first. Recursing on `n / base` before emitting
  `n % base` puts them in the right order with no buffer reversal.
- `sort_ints`: try a quicksort-shaped scheme — partition around a
  pivot, then recurse on each side. (Any correct sort passes; the
  largest test array has 300 elements.)

Examples the harness checks (among others):

| call                    | result       |
|-------------------------|--------------|
| `fmt_base(0, 10, out)`  | `"0"`        |
| `fmt_base(255, 16, out)`| `"ff"`       |
| `fmt_base(255, 2, out)` | `"11111111"` |
| `fmt_base(7, 8, out)`   | `"7"`        |

The grader runs the harness twice: once normally, then again built
with AddressSanitizer and UBSan — an out-of-bounds write or any
undefined behaviour fails the task even if the answers look right.

Grade with `ctrain check radix-and-sort`.
