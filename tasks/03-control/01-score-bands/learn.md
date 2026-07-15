THE IDEA

  Programs make decisions. The tool for that is `if`: run some steps only
  when a condition is true, otherwise do something else.

      if (condition) {
          ...runs when the condition is true...
      } else {
          ...runs otherwise...
      }

  A condition is any test that is true or false. The comparison operators:

      a < b   less than        a <= b  less than or equal
      a > b   greater than     a >= b  greater than or equal
      a == b  equal (note: TWO equals signs)   a != b  not equal

  You can combine tests: `||` means OR, `&&` means AND. So "out of range"
  is `score < 0 || score > 100`.

---

  Here is a decision in action:

```run
#include <stdio.h>

int main(void)
{
    int score = 72;
    if (score < 0 || score > 100) {
        printf("%d: invalid\n", score);
    } else {
        printf("%d: in range\n", score);
    }
    return 0;
}
```

  Try changing 72 to 150 in your head: the first condition (score > 100)
  becomes true, so it would print "invalid" instead.

---

CHAINING DECISIONS

  When there are several bands, chain the tests with `else if`. C checks them
  top to bottom and takes the FIRST one that is true, then skips the rest —
  so you order them from the highest band down:

```run
#include <stdio.h>

int main(void)
{
    int score = 85;
    char letter;
    if (score >= 90)      letter = 'A';
    else if (score >= 80) letter = 'B';
    else if (score >= 70) letter = 'C';
    else if (score >= 60) letter = 'D';
    else                  letter = 'F';
    printf("%d: %c\n", score, letter);
    return 0;
}
```

  85 fails `>= 90` but passes `>= 80`, so it stops there: B. `char` holds a
  single character like 'B', and `%c` is the placeholder that prints one
  character. Ordering top-down is what lets each test assume the ones above
  already failed.

---

WHY IT MATTERS

  Branching on ranges — grading, pricing tiers, thresholds, validation — is
  the bread and butter of program logic. The two habits here (validate the
  input first, then order your bands so the first match wins) prevent whole
  classes of bugs.

---

THE TERNARY OPERATOR

  For a simple "one value if true, another if false", there's a compact
  form: `condition ? value-if-true : value-if-false`. It is an expression,
  so you can drop it straight into printf. Pass/fail is a perfect fit:

```run
#include <stdio.h>

int main(void)
{
    int score = 55;
    printf("%d: %s\n", score, score >= 60 ? "pass" : "fail");
    return 0;
}
```

  `%s` prints a string of text (here either "pass" or "fail"). The `?:` picks
  which string based on the test. You could write it as a full if/else
  instead — use whichever reads more clearly.

---

READING UNTIL END OF INPUT

  This task reads many scores, not one. `scanf("%d", &score)` reads one whole
  number from the input into `score`, and returns 1 when it succeeds. Looping
  while that is 1 processes every number until the input runs out:

      while (scanf("%d", &score) == 1) {
          ...handle this score...
      }

  (The `&` before score means "the address of score" — where to store what
  was read. You'll study that properly in the pointers chapter; for now,
  scanf needs the & so it can put the value into your variable.)

---

CHECK IT WORKED

  Put it together: loop reading scores; for each, print `s: invalid` when it
  is below 0 or above 100, otherwise the letter band and `(pass)`/`(fail)`.
  The grader feeds sample inputs and checks your exact lines, including the
  boundary values (90, 80, 70, 60) and empty input (which prints nothing).

---

GOTCHAS

  - Use `==` for comparison, not `=`. A single `=` assigns a value and is a
    common, quiet bug.
  - Validate the range FIRST and skip the rest for that score, or an invalid
    value like 150 would also get a letter band.
  - Order the bands high-to-low. If you tested `>= 60` first, everything 60+
    would come out as D.
  - Empty input must print nothing at all — the while loop handles that
    naturally, since scanf returns non-1 immediately and the body never runs.
