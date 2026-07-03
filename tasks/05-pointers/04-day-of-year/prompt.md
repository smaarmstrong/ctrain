# day of year

Convert between calendar dates and day-of-year numbers using a
two-dimensional lookup table: one row of month lengths for ordinary
years, one for leap years.

A year is a **leap year** when it is divisible by 4 but not by 100, or
when it is divisible by 400. So 2024 and 2000 are leap years; 2023 and
1900 are not.

Implement these two functions in `main.c`. Do **not** define `main` —
the grader links your file against a test harness that provides it.

```c
/* Day number within the year: 1 January is 1, 31 December is 365
 * (366 in a leap year).
 * Return -1 if month is not 1..12 or day is not a valid day of that
 * month in that year (so day_of_year(2023, 2, 29) is -1 but
 * day_of_year(2024, 2, 29) is 60). */
int day_of_year(int year, int month, int day);

/* The inverse: given a day number 1..365 (366 in a leap year), store
 * the month in *pmonth (1..12) and the day of the month in *pday, and
 * return 0.
 * If yearday is out of range for that year, return -1 and leave
 * *pmonth and *pday unchanged. */
int month_day(int year, int yearday, int *pmonth, int *pday);
```

Notes and edge cases:

- Years passed by the grader are always ≥ 1; you only validate month,
  day, and yearday.
- The natural data structure is
  `static const int table[2][13]` (or `[2][12]`) — one row per
  leap-ness. Any correct implementation passes; the table is a hint,
  not a requirement.
- Round trips must hold: for every valid date,
  `month_day(y, day_of_year(y, m, d), &m2, &d2)` gives back `m2 == m`
  and `d2 == d`. The grader checks the full years 2023 and 2024.
- Do not index outside your table — the grader re-runs everything
  under AddressSanitizer/UBSan.

Grade with `ctrain check day-of-year`.
