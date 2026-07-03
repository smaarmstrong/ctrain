# generic sort

The standard library's `qsort` sorts *anything* because it knows
nothing about the element type: it sees the array as raw bytes and
delegates every comparison to a function pointer supplied by the
caller. Build that yourself.

Implement this function in `main.c`. Do **not** define `main` — the
grader links your file against a test harness that provides it.

```c
#include <stddef.h>

/* Sort the array at base, containing n elements of size bytes each,
 * into ascending order as defined by cmp.
 *
 * cmp receives pointers to two elements and returns negative, zero
 * or positive exactly like strcmp. Elements comparing equal may end
 * up in any relative order (stability is not required).
 *
 * n may be 0 (base may then be NULL) or 1 — both are no-ops.
 */
void gsort(void *base, size_t n, size_t size,
           int (*cmp)(const void *, const void *));
```

Rules:

- **You may not call `qsort`** (or `qsort_r`/`qsort_s`) — the grader
  rejects your file if the identifier `qsort` appears anywhere in it,
  comments included. Any sort you write yourself is fine; with the
  test sizes (at most 1000 elements) even insertion sort finishes
  instantly.
- Your code must not depend on the element type: move elements around
  with byte operations (`memcpy`/`memmove` on `size` bytes, or a
  byte-by-byte swap loop through `unsigned char *`).
- Compare elements **only** through `cmp`, and pass it pointers to
  elements *inside the array*.
- Never touch memory outside `[base, base + n*size)` — the grader
  re-runs the harness under AddressSanitizer/UBSan. If you allocate
  scratch space, free it.

The harness sorts `int`s (ascending and descending), `double`s,
strings (an array of `const char *` compared with `strcmp` through
the comparator), and a small struct by one field — all with the same
`gsort`.

Grade with `ctrain check generic-sort`.
