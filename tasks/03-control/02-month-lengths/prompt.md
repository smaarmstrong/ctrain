# Month lengths with a fallthrough switch

Implement these two functions in `main.c`. Do **not** define `main` —
the grader links your file against a test harness that provides it.

```c
/* Return 1 if year is a leap year in the Gregorian calendar, else 0.
 * Rule: divisible by 4, except centuries, except every 4th century.
 * So 1996 and 2000 are leap years; 1900 and 2023 are not.
 * year is always >= 1. */
int is_leap(int year);

/* Return the number of days in the given month of the given year:
 * 28, 29, 30 or 31. February depends on is_leap.
 * Months are numbered 1 (January) to 12 (December).
 * Return -1 for any month outside 1..12. */
int days_in_month(int month, int year);
```

The intended shape for `days_in_month` is a `switch` where the seven
31-day months share one body and the four 30-day months share another —
adjacent empty `case` labels falling through on purpose:

```c
case 1: case 3: /* ... */
    return 31;
```

Deliberate, labelled fallthrough like this is the one place fallthrough
is good style; falling out of a case that *has* a body is where the bugs
live. The grader checks behaviour only, so an if/else version also
passes — but write the switch, it is what this exercise is about.

Behaviour the harness probes:

- all seven 31-day months (1, 3, 5, 7, 8, 10, 12),
- all four 30-day months (4, 6, 9, 11),
- February in leap and non-leap years, including the century cases
  (1900 → 28, 2000 → 29),
- invalid months (0, 13, -2) → -1 regardless of year,
- `is_leap` on its own: 2024 → 1, 2023 → 0, 1900 → 0, 2000 → 1.

Grade with `ctrain check month-lengths`.
