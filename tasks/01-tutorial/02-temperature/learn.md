THE IDEA

  Two new tools turn a one-line program into a real one: a loop, so the
  computer repeats work for you, and formatted numbers, so the output lines
  up neatly.

  A `for` loop has three parts in its parentheses, separated by semicolons:

      for (start ; keep-going-while ; step) {
          ...body, repeated each time...
      }

  Read it as: "start here; as long as this is true, run the body; then do
  the step, and check again." For example, count 0, 2, 4 ... up to 6:

      for (int i = 0; i <= 6; i += 2) { ... }

  `int i = 0` makes a whole-number variable i starting at 0. `i <= 6` is the
  condition to keep going. `i += 2` is shorthand for "add 2 to i".

---

  Let's watch a loop run. This prints five numbers, one per line:

```run
#include <stdio.h>

int main(void)
{
    for (int n = 0; n <= 80; n += 20) {
        printf("%d\n", n);
    }
    return 0;
}
```

  `%d` inside the printf text is a placeholder that means "put a whole
  number (an int) here"; the value to drop in (n) is given after the string.
  So each time round the loop, the current n replaces `%d`.

---

WHY IT MATTERS

  Tables of numbers — conversions, prices, measurements — are everywhere,
  and a loop with formatted output is how you produce them without writing
  each line by hand. This exact program (a Fahrenheit-to-Celsius table) is
  the classic first loop from the C book for that reason.

---

THE FLOATING-POINT TRAP

  The conversion is C = (5/9)(F - 32). But here is a trap that catches
  everyone once. In C, dividing two whole numbers throws away the remainder:
  5 / 9 is 0, not 0.5556. So `5 / 9` would make every Celsius value zero.

  The fix is to do the maths in "floating point" — numbers with a decimal
  point. Writing `5.0 / 9.0` (with the .0) tells C these are real numbers,
  so it keeps the fraction. See the difference:

```run
#include <stdio.h>

int main(void)
{
    printf("whole-number division 5/9   = %d\n", 5 / 9);
    printf("floating division 5.0/9.0   = %f\n", 5.0 / 9.0);
    return 0;
}
```

  `%f` is the placeholder for a floating-point number (a `double`). Note it
  printed lots of digits — next we'll control that.

---

CONTROLLING THE COLUMNS

  Inside a placeholder you can ask for width and precision:

    %3d    a whole number in a field at least 3 wide (padded with spaces)
    %6.1f  a float in a field 6 wide, with exactly 1 digit after the point

  Width pads on the left, so numbers line up in a right-aligned column.
  Watch:

```run
#include <stdio.h>

int main(void)
{
    double c = -17.777;
    printf("[%3d] [%6.1f]\n", 0, c);
    printf("[%3d] [%6.1f]\n", 300, 148.9);
    return 0;
}
```

  See how the numbers sit in tidy columns, and the floats show just one
  decimal place. That "%3d %6.1f" pattern is exactly the shape this task's
  output needs.

---

CHECK IT WORKED

  The grader diffs your whole table against the expected text, so both the
  numbers and the spacing must match. Loop F from 0 to 300 inclusive in
  steps of 20, compute Celsius with `(5.0 / 9.0) * (fahr - 32)`, and print
  each line with `"%3d %6.1f\n"` — the F value, a space, the Celsius value.

---

GOTCHAS

  - The integer-division trap: use 5.0 / 9.0, not 5 / 9. If your whole
    column reads 0.0, this is why.
  - `<= 300`, not `< 300`: the table must include the 300 row.
  - Put a space between the two placeholders in the format string — the
    columns are separated by exactly one space.
  - Store Celsius in a `double` (a floating type), not an `int`, or you lose
    the decimals before you even print them.
