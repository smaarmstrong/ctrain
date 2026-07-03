# months as an enumeration

A calendar month is a classic closed set: exactly twelve named values.
Implement the three functions below in `main.c`. Do **not** define
`main` — the grader links your file against a test harness that
provides it.

Months are numbered 1 (January) through 12 (December). Declare an
`enum` for them so the numbers have names in your code — the harness
only sees the functions, but the enum is the point of the exercise.

```c
#include <stdbool.h>

/* Full English month name: "January", "February", ..., "December".
 * For any m outside 1..12 return the string "?". */
const char *month_name(int m);

/* Number of days in month m: 28/29 for February depending on leap,
 * 30 or 31 elsewhere. Return 0 for m outside 1..12. */
int days_in_month(int m, bool leap);

/* 1-based ordinal of the date within its year: day_of_year(1, 1, x)
 * is 1, day_of_year(12, 31, false) is 365, day_of_year(12, 31, true)
 * is 366. Return -1 whenever m is outside 1..12 or d is outside the
 * valid days for that month/leap combination. */
int day_of_year(int m, int d, bool leap);
```

Notes:

- `month_name` must return a pointer that stays valid after the call —
  return string literals (or pointers into a `static` array of them),
  never the address of a local buffer, and never freshly allocated
  memory (the harness never frees what you return, so an allocation
  is a leak).
- Capitalisation matters: `"January"`, not `"january"`.
- The harness also rebuilds everything with AddressSanitizer and
  UBSan; a dangling return or a leak fails the task.

Grade with `ctrain check months-and-enums`.
