# swap, reverse, clone

Implement these three functions in `main.c`. Do **not** define `main` —
the grader links your file against a test harness that provides it.

```c
#include <stddef.h>

/* Exchange the two ints the arguments point to.
 * Must behave correctly even when a and b point to the same object. */
void swap(int *a, int *b);

/* Reverse the first n elements of a in place.
 * n may be 0; a may be NULL when n is 0. */
void reverse(int *a, size_t n);

/* Return a pointer to a freshly allocated copy of the first n ints of src.
 * The CALLER owns the memory and will free() it.
 * Return NULL if n is 0 or the allocation fails. */
int *clone_ints(const int *src, size_t n);
```

The harness checks behaviour first, then runs everything again under
AddressSanitizer and UBSan: an allocation that is too small, a read past
the end, or a leak fails the task even if the answers look right.

Grade with `ctrain check swap-reverse-clone`.
