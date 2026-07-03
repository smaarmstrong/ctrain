# stats — read before write

A local variable in C starts with an *indeterminate* value: whatever
bytes happen to be lying around on the stack. Reading it before you
assign to it is undefined behaviour. The cruel part is that the
leftover bytes are often zero, so the bug "works" on your machine and
explodes somewhere else. Compilers catch some of these
(`-Wuninitialized`), but far from all — including the two in this task,
which compile silently even with `-Wall -Wextra -Werror`.

`main.c` contains two small statistics helpers. Each one reads a
variable that was never given a starting value. Find both and fix them
so the functions meet this contract:

```c
#include <stddef.h>

/* Return the largest of the first n elements of a.
 * n is always >= 1. Works for all-negative arrays too. */
int max_of(const int *a, size_t n);

/* Return the sum of the first n elements of a, as a long long
 * (the sum may not fit in an int). n may be 0; the sum of nothing
 * is 0. */
long long sum_of(const int *a, size_t n);
```

Examples the grader checks (among others):

- `max_of((int[]){9, 2, 5}, 3)` → `9` — the first element counts too.
- `max_of((int[]){-7}, 1)` → `-7`
- `sum_of(a, 0)` → `0`
- `sum_of((int[]){1000000000, 1000000000, 1000000000}, 3)` →
  `3000000000`

Do **not** define `main` — the grader links your file against a test
harness. To stop an uninitialised variable from getting lucky, the
harness deliberately smears junk bytes across the stack right before
every call, so "it happened to be zero" will not save the buggy code.

Grade with `ctrain check uninitialised-stats`.
