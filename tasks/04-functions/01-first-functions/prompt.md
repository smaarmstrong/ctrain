# three small functions

Implement the three functions below in `main.c`. Do **not** define
`main` — the grader links your file against a test harness that
provides it. The harness declares exactly these prototypes, so your
definitions must match them name for name and type for type:

```c
/* base raised to the power exp, computed in integer arithmetic.
 * exp is never negative. Anything to the power 0 is 1, including
 * ipow(0, 0) == 1. Results always fit in a long. */
long ipow(int base, int exp);

/* x limited to the range [lo, hi]: return lo when x < lo,
 * hi when x > hi, otherwise x itself. lo <= hi always holds. */
int clampi(int x, int lo, int hi);

/* The number of decimal digits in n, ignoring any minus sign:
 * digit_count(0) == 1, digit_count(7) == 1, digit_count(-12345) == 5. */
int digit_count(long n);
```

Notes:

- A *prototype* promises the compiler a function's name, parameter
  types and return type; the *definition* supplies the body. If your
  definition disagrees with the harness's prototype, the build fails.
- You may add helper functions of your own; declare them (or define
  them) before first use.
- The harness never passes a negative `exp`, and never passes
  `LONG_MIN` to `digit_count`.

The grader compiles your file together with the harness under
`-std=c11 -Wall -Wextra -Werror` and runs the harness's checks; every
check must pass.

Grade with `ctrain check first-functions`.
