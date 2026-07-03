# Function-like macros without the classic traps

A macro is textual substitution, not a function call. Written carelessly,
`SQ(1 + 2)` becomes `1 + 2 * 1 + 2` and quietly computes 5 instead of 9.

Define these four macros in `main.c`. Do **not** define `main` — the
grader textually `#include`s your file into a test harness that provides
it, then expands your macros inside larger expressions.

```c
SQ(x)              /* x squared                                   */
MIN(a, b)          /* the smaller of a and b                      */
MAX(a, b)          /* the larger of a and b                       */
CLAMP(x, lo, hi)   /* lo if x < lo, hi if x > hi, otherwise x     */
```

Requirements:

- All arguments are `int`-valued expressions and each result must be an
  `int`-valued expression.
- Each macro must behave as if it were a function: it must produce the
  right value **no matter what expression is passed as an argument and no
  matter what expression surrounds the expansion**. The harness probes
  exactly these situations, for example:

  ```c
  SQ(1 + 2)            /* must be 9  */
  72 / SQ(2 + 1)       /* must be 8  */
  -SQ(1 + 1)           /* must be -4 */
  MIN(2, 3) * 10       /* must be 20 */
  10 - MAX(1, 5)       /* must be 5  */
  100 / CLAMP(3, 1, 4) /* must be 33 */
  ```

- You may assume arguments have **no side effects** (nothing like `i++`),
  so evaluating an argument more than once is acceptable here.
- Any helper macros you like are allowed; the four names above must exist
  as function-like macros or as functions — only the resulting values are
  checked.

The starter file contains four superficially plausible definitions that
compile but fail several of the probes above. Fix them.

Grade with `ctrain check paren-pitfalls`.
