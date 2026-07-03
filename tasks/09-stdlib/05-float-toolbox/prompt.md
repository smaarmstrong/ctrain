# floating-point toolbox — classify and round

Doubles are not just "numbers": the format also contains two infinities,
NaNs, signed zeros and a band of tiny *subnormal* values. `<math.h>`
gives you tools that handle all of them; `(long long)(x + 0.5)` and
friends do not.

Implement these three functions in `main.c`. Do **not** define `main` —
the grader links your file against a test harness that provides it.

```c
/* Return a string literal naming the class of x:
 *   "nan"        x is any NaN
 *   "+inf"       positive infinity
 *   "-inf"       negative infinity
 *   "zero"       +0.0 or -0.0
 *   "subnormal"  nonzero but below the normal range (e.g. 5e-324)
 *   "normal"     every ordinary value
 * Use fpclassify/isnan/isinf — comparisons like x == x are fragile
 * and won't cover subnormals anyway. */
const char *fp_class(double x);

/* Round x to the nearest integer, halves AWAY from zero:
 *   2.5 -> 3,  -2.5 -> -3,  2.4 -> 2,  -2.6 -> -3
 * The harness only passes finite x whose result fits in long long.
 * (math.h has a single function that does exactly this.) */
long long ll_round(double x);

/* Round x to `places` decimal digits (0 <= places <= 9), halves away
 * from zero, and return the result as a double:
 *   round_places(1.25, 1)    -> 1.3
 *   round_places(-1.25, 1)   -> -1.3
 *   round_places(3.14159, 2) -> 3.14
 *   round_places(2.5, 0)     -> 3.0
 * Scale up by a power of 10, round, scale back down. Results are
 * compared with a tolerance of 1e-9, so the usual scale-round-unscale
 * approach passes; exact decimal representation is not expected. */
double round_places(double x, int places);
```

`fp_class` returns pointers to string literals — do not allocate. The
harness runs everything again under the sanitizers, so undefined
behaviour in float handling (e.g. casting an out-of-range double to an
integer type yourself) fails the task.

Grade with `ctrain check float-toolbox`.
