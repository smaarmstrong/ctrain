THE IDEA

  Numbers in C come in different types, and each type can only hold a limited
  range of values, because each is stored in a fixed number of bits. An
  `int` is a whole number of some fixed size; an `unsigned int` is a whole
  number that is never negative (so it can count higher); a `double` is a
  real number with a fractional part.

  You do not have to memorise the ranges. C ships them as named constants in
  two standard headers:

      #include <limits.h>    ranges of the integer types: INT_MAX, UINT_MAX...
      #include <float.h>     facts about the floating types: DBL_DIG...

  Using the named constant (INT_MAX) instead of typing a number means your
  program is correct on any machine, and says what you mean.

---

  Let's print a few of them. Each needs the placeholder that matches its
  type — we'll come back to which is which:

```run
#include <limits.h>
#include <stdio.h>

int main(void)
{
    printf("CHAR_BIT %d\n", CHAR_BIT);   /* how many bits are in one char */
    printf("INT_MIN %d\n", INT_MIN);     /* most negative int */
    printf("INT_MAX %d\n", INT_MAX);     /* most positive int */
    return 0;
}
```

  CHAR_BIT is almost always 8. INT_MIN and INT_MAX are the edges of what an
  int can hold.

---

WHY IT MATTERS

  Overflow bugs — a value quietly growing past its type's maximum — are a
  classic source of real crashes and security holes. Knowing where the
  limits are, and reading them from the headers rather than guessing, is the
  first step to writing code that doesn't fall off those edges.

---

MATCHING THE PLACEHOLDER TO THE TYPE

  Here is the rule that this whole task drills: each type has its own printf
  placeholder, and using the wrong one is a bug. The ones you need:

      int         %d
      unsigned    %u        (never-negative whole number)
      long long   %lld      (an extra-wide whole number)
      size_t      %zu       (the type sizeof gives you)
      double      %g        (a real number, compactly)

  `sizeof(x)` tells you how many bytes a type takes; its result is a
  `size_t`, so it prints with %zu. And UINT_MAX doesn't fit in an int, so it
  needs %u, not %d:

```run
#include <limits.h>
#include <stdio.h>

int main(void)
{
    printf("sizeof(int) %zu\n", sizeof(int));
    printf("UINT_MAX %u\n", UINT_MAX);
    return 0;
}
```

  This matters more than it looks: the grader compiles with -Wall -Wextra
  -Werror, so a mismatched placeholder (say %d for an unsigned) is treated
  as an error and your code won't build.

---

UNSIGNED WRAP-AROUND

  One more idea the task shows. Unsigned numbers don't overflow into an
  error — they wrap around, like a car odometer rolling from 999999 back to
  0. So the largest unsigned value plus one becomes zero. The `u` on `1u`
  keeps the whole sum in unsigned arithmetic:

```run
#include <limits.h>
#include <stdio.h>

int main(void)
{
    printf("UINT_MAX   %u\n", UINT_MAX);
    printf("plus one   %u\n", UINT_MAX + 1u);
    return 0;
}
```

  It wrapped to 0 — that is defined, deliberate behaviour for unsigned types
  (unlike signed overflow, which you'll meet later as a genuine hazard).

---

CHECK IT WORKED

  The task asks for twelve specific lines in the format `NAME value`, each
  value taken from the standard macros or from sizeof, each printed with the
  matching placeholder. The grader diffs your output against the expected
  text. Include <limits.h> for the integer macros and <float.h> for the
  floating ones (FLT_DIG, DBL_EPSILON, ...).

---

GOTCHAS

  - Wrong placeholder = compile error here (because of -Werror). If it won't
    build, check each line's type against the table above.
  - sizeof gives a size_t: use %zu, not %d.
  - FLT_EPSILON is a float; passing it to %g promotes it to double, which is
    what %g expects — printf("%g", (double)FLT_EPSILON) is the safe form.
  - Read the values from the macros; don't type the numbers, they differ
    between machines.
