THE IDEA

  This task is about two things at once: doing arithmetic on variables, and
  controlling exactly how printf writes the results.

  A variable is a named box holding a value. You make one by giving its type
  and a starting value:

      int a = 1234;
      int b = 56;

  `int` means "whole number". Once a and b exist, you can compute with them
  using the arithmetic operators:

      a + b   add        a - b   subtract
      a * b   multiply   a / b   divide   a % b   remainder ("modulo")

---

  Let's compute with two variables and print the results. Each `%d` in the
  text is filled, in order, by the values listed after the string:

```run
#include <stdio.h>

int main(void)
{
    int a = 1234;
    int b = 56;
    printf("a=%d b=%d\n", a, b);
    printf("sum=%d diff=%d\n", a + b, a - b);
    return 0;
}
```

  The key idea: you pass printf the *expressions* (a + b), and it prints
  their computed values. You never type the answer yourself — the program
  works it out.

---

INTEGER DIVISION VS REAL DIVISION

  With whole numbers, `/` gives the quotient and `%` gives the remainder,
  both throwing away any fraction: 1234 / 56 is 22, and 1234 % 56 is 2
  (because 22 * 56 = 1232, leaving 2).

  To get the true decimal answer you must divide in floating point. You turn
  an int into a floating value with a "cast": `(double)a` means "treat a as
  a double for this calculation". Then use the `%f` placeholder, and `.3f`
  to ask for exactly three digits after the point:

```run
#include <stdio.h>

int main(void)
{
    int a = 1234;
    int b = 56;
    printf("quot=%d rem=%d\n", a / b, a % b);
    printf("ratio=%.3f\n", (double)a / b);
    return 0;
}
```

  Once one side is a double, C does the whole division in floating point, so
  `(double)a / b` keeps the fraction.

---

WHY IT MATTERS

  Almost every program computes values and then presents them to a human:
  totals, averages, percentages, aligned columns. printf's little format
  language is how C does that, and reading a format string fluently — width,
  precision, literal signs — pays off constantly.

---

TWO MORE PLACEHOLDER TRICKS

  A percentage needs a literal % sign in the output. But % starts a
  placeholder, so to print an actual percent sign you write it twice: `%%`.

  And to right-align a number in a column, put a width in the placeholder:
  `%6d` prints in a field 6 characters wide, padding with spaces on the left.

```run
#include <stdio.h>

int main(void)
{
    int a = 1234;
    int b = 56;
    printf("pct=%.2f%%\n", 100.0 * b / a);
    printf("boxed=[%6d]\n", a);
    return 0;
}
```

  In `pct=`, the `%.2f` prints the number with two decimals and the trailing
  `%%` prints one literal percent sign. In `boxed=`, `%6d` right-aligns 1234
  in six columns, so two spaces pad it: `[  1234]`.

---

CHECK IT WORKED

  The grader compares your six lines byte-for-byte with the expected output,
  so spacing and precision must be exact. Compute every value from a and b —
  the whole point is that the program does the arithmetic, not you.

---

GOTCHAS

  - Integer vs floating division: line 3 wants the *integer* quotient/rem
    (a / b, a % b); line 4 wants the *floating* ratio ((double)a / b). Mixing
    them up is the classic error here.
  - `%%` prints one percent sign. A single `%` in a format string starts a
    placeholder and will misbehave.
  - `%6d` pads on the left (right-aligned). Two spaces before 1234 is what
    "field width 6" gives you, since 1234 is four digits.
  - Don't paste the numbers in as text. `printf("sum=1290\n")` would match
    the output but defeats the task — compute a + b.
