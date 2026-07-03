# conversion traps

C converts between integer types silently, and two of those silent
conversions account for an outsized share of real-world bugs:

1. **Unsigned underflow.** `size_t` is unsigned, so it can never go
   below zero — it *wraps*. If `len` is a `size_t` holding `0`, then
   `len - 1` is not `-1`; it is `SIZE_MAX`, the largest value the type
   can hold. A loop bounded by `i <= len - 1` therefore runs
   (effectively) forever and marches straight off the end of the
   array — an out-of-bounds read, which is undefined behaviour.

2. **`char` vs `EOF`.** `fgetc` returns an `int` for a reason: it must
   be able to return every byte value `0..255` *plus* the distinct
   sentinel `EOF` (a negative value, typically `-1`). Store the result
   in a `char` and that distinction is destroyed. On platforms where
   `char` is signed, the byte `0xFF` becomes `-1` and compares equal
   to `EOF` — the loop stops in the middle of the data. Where `char`
   is unsigned, `EOF` itself becomes `255` and the loop never stops.
   Either way the program is wrong.

The starter `main.c` contains one function with each bug. Both
functions compile without a single warning. Fix them so they honour
these contracts exactly — keep the signatures unchanged:

```c
#include <stddef.h>
#include <stdio.h>

/* Return how many of the first len elements of a are >= cutoff.
 * len == 0 is valid and must return 0 without reading a[0] —
 * a may not point at any readable element at all. */
size_t count_at_least(const int *a, size_t len, int cutoff);

/* Read fp to true end-of-file and return the number of '\n' bytes
 * seen. Every byte value 0..255 may appear in the stream, including
 * 0xFF; none of them may be mistaken for end-of-file. An empty
 * stream has 0 lines. */
size_t count_newlines(FILE *fp);
```

Examples:

- `count_at_least((int[]){5, 9, 2}, 3, 5)` returns `2`.
- `count_at_least(p, 0, 42)` returns `0` for any `p`, touching nothing.
- `count_newlines` on a stream holding `"a\n\xFF\xFFb\nc\n"` returns
  `3` — the two `0xFF` bytes are ordinary data, not end-of-file.
- `count_newlines` on an empty stream returns `0`; a stream whose last
  line has no trailing `'\n'` counts only the newlines actually present.

A related trap worth knowing (not graded here): the `<ctype.h>`
functions such as `isalpha(c)` have the same shape — their argument
must be an `unsigned char` value or `EOF`, so passing a plain `char`
that happens to be negative is undefined behaviour. The idiomatic
fix is a cast: `isalpha((unsigned char)c)`.

Do **not** define `main` — the grader links your file against a test
harness. The harness calls `count_at_least` with `len == 0`, feeds
`count_newlines` streams containing `0xFF` bytes, and repeats every
call under AddressSanitizer, where the wrapped loop bound is caught
the moment it reads one element too many.

Grade with `ctrain check conversion-traps`.
