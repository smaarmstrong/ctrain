# checked arithmetic

Signed integer overflow is **undefined behaviour** in C. When `a + b`
does not fit in an `int`, the standard does not say the result wraps
around — it says anything may happen. Optimisers exploit this: a
"check afterwards" test like `if (a + b < a)` can be deleted entirely,
because the compiler is allowed to assume the overflow never occurred.
The only safe pattern is to decide *before* doing the arithmetic whether
the result would fit.

Implement these three functions in `main.c`. Do **not** define `main` —
the grader links your file against a harness that provides it.

```c
#include <stdbool.h>

/* Each function computes a OP b.
 * If the mathematical result fits in an int: store it in *result and
 * return true.
 * Otherwise: return false and leave *result completely untouched. */
bool add_ok(int a, int b, int *result);
bool sub_ok(int a, int b, int *result);
bool mul_ok(int a, int b, int *result);
```

Rules the grader enforces:

- On overflow you must return `false` **and not write to `*result`** —
  the harness plants a sentinel value there and checks it survives.
- Your code must never execute a signed operation that overflows. The
  whole run is repeated under UBSan; a single
  `signed integer overflow` report fails the task even if every return
  value looked right.
- Any *defined-behaviour* strategy is fine: compare against limits from
  `<limits.h>` before operating (e.g. `a > INT_MAX - b`), or do the
  arithmetic in a wider type such as `long long` and range-check the
  result before narrowing.

Cases the harness probes include: ordinary sums/products, `INT_MAX` and
`INT_MIN` at both ends, `INT_MIN - 1` via subtraction, `0 - INT_MIN`,
`INT_MIN * -1`, and multiplications just above and just below the
32-bit boundary.

Grade with `ctrain check checked-arithmetic`.
