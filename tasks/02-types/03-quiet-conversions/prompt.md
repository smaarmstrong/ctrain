# quiet conversions, loud bugs

C converts between numeric types silently, and the silence is where the
bugs live: integer division where you wanted a fraction, intermediate
results that overflow before the widening you expected, doubles rammed
into ints they cannot fit. Each function below is trivial arithmetic —
the entire exercise is placing the casts so every intermediate value is
computed in the right type.

Implement these in `main.c`. Do **not** define `main` — the grader
links your file against a test harness that provides it.

```c
#include <stddef.h>

/* Arithmetic mean of the first n elements, as a double.
 * Return 0.0 when n is 0. The elements may be anywhere in the int
 * range and n may be large enough that the sum does not fit in an
 * int — pick a wider type for the accumulator. */
double mean_of(const int *a, size_t n);

/* The integer percentage (part * 100) / whole, truncated toward zero.
 * whole is never 0. Must be exact even when part * 100 overflows int
 * (e.g. percent_of(50000000, 200000000) is 25): do the multiplication
 * in a 64-bit type. */
int percent_of(int part, int whole);

/* The low 8 bits of v, as a value 0..255. Must be well defined for
 * negative v: low_byte(-1) is 255, low_byte(-256) is 0. (Conversion
 * to an unsigned type is defined to wrap — use that.) */
unsigned char low_byte(long v);

/* d converted to int: truncated toward zero when it fits, saturated
 * to INT_MAX / INT_MIN when it is beyond the int range in either
 * direction. clamp_to_int(3.9) is 3, clamp_to_int(-3.9) is -3,
 * clamp_to_int(1e18) is INT_MAX, clamp_to_int(-1e18) is INT_MIN.
 * Casting an out-of-range double straight to int is undefined
 * behaviour — compare first, cast after. */
int clamp_to_int(double d);
```

The harness checks values first, then rebuilds with UBSan and
AddressSanitizer and runs everything again: a signed overflow or an
out-of-range float-to-int cast fails the task even if the answer
happened to look right.

Grade with `ctrain check quiet-conversions`.
