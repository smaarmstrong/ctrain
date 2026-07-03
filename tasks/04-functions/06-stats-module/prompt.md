# split a program into a module: stats.h / stats.c

Real programs are split across files: a *header* declares what a
module offers, an *implementation file* defines it, and other files
`#include` the header to use it. Your work directory contains three
files — keep these exact names, the grader compiles them by name:

- `main.c` — the driver, **provided complete**; you should not need
  to change it.
- `stats.h` — the module's interface. It must define `struct stats`
  and declare `stats_of` (already sketched), and it must be protected
  by an **include guard** so that including it twice in one file still
  compiles. The grader builds a test program that does exactly that:
  `#include "stats.h"` twice in a row.
- `stats.c` — implement `stats_of` here.

## The interface

`stats.h` must provide exactly this type and function (field names
and types matter — the grader's own test file uses them):

```c
#include <stddef.h>

struct stats {
    size_t count;   /* how many values                    */
    long   min;     /* smallest value; 0 when count == 0  */
    long   max;     /* largest value; 0 when count == 0   */
    long   sum;     /* total; always fits in a long       */
    double mean;    /* sum / count; 0.0 when count == 0   */
};

/* Summarise the first n longs at values. n may be 0, in which
 * case values may be NULL and every field comes back zero. */
struct stats stats_of(const long *values, size_t n);
```

## What the whole program does

`main.c` reads up to 1000 whitespace-separated longs from standard
input, calls `stats_of`, and prints either

```
count 5
min -3
max 12
sum 20
mean 4.00
```

(that is the output for input `5 -3 12 0 6`; the mean is printed with
two digits after the decimal point) — or, when there was no input at
all, the single line `no data`, exiting with status 1 instead of 0.
That behaviour is already written; it works as soon as `stats_of` is
correct.

## Grading

1. `main.c` + `stats.c` are compiled together and the program is run
   on several inputs, including empty input.
2. A grader-supplied test file that includes `stats.h` **twice** is
   compiled against your `stats.c` and calls `stats_of` directly
   (including `stats_of(NULL, 0)`). No include guard → no compile.
3. The same test program is rebuilt under AddressSanitizer/UBSan; any
   out-of-bounds read or undefined behaviour fails the task.

Grade with `ctrain check stats-module`.
