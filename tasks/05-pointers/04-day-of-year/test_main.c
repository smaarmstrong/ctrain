/* Test harness — provides main() and calls the learner's functions. */
#include <stdio.h>

int day_of_year(int year, int month, int day);
int month_day(int year, int yearday, int *pmonth, int *pday);

static int failures;

#define CHECK(cond, msg)                                        \
    do {                                                        \
        if (cond) {                                             \
            printf("  ok    %s\n", msg);                        \
        } else {                                                \
            printf("  FAIL  %s\n", msg);                        \
            failures++;                                         \
        }                                                       \
    } while (0)

static int is_leap(int y)
{
    return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
}

int main(void)
{
    /* day_of_year — fixed points */
    CHECK(day_of_year(2023, 1, 1) == 1, "1 Jan is day 1");
    CHECK(day_of_year(2023, 12, 31) == 365, "31 Dec 2023 is day 365");
    CHECK(day_of_year(2024, 12, 31) == 366, "31 Dec 2024 is day 366");
    CHECK(day_of_year(2023, 3, 1) == 60, "1 Mar in an ordinary year is day 60");
    CHECK(day_of_year(2024, 3, 1) == 61, "1 Mar in a leap year is day 61");
    CHECK(day_of_year(2024, 2, 29) == 60, "29 Feb 2024 is day 60");
    CHECK(day_of_year(2000, 2, 29) == 60, "2000 is a leap year (divisible by 400)");
    CHECK(day_of_year(1900, 2, 29) == -1, "1900 is NOT a leap year (divisible by 100)");

    /* day_of_year — rejects bad input */
    CHECK(day_of_year(2023, 2, 29) == -1, "29 Feb 2023 is invalid");
    CHECK(day_of_year(2023, 0, 10) == -1, "month 0 is invalid");
    CHECK(day_of_year(2023, 13, 1) == -1, "month 13 is invalid");
    CHECK(day_of_year(2023, 6, 0) == -1, "day 0 is invalid");
    CHECK(day_of_year(2023, 4, 31) == -1, "31 Apr is invalid");
    CHECK(day_of_year(2023, 1, 32) == -1, "32 Jan is invalid");

    /* month_day — fixed points */
    int m = -7, d = -7;
    CHECK(month_day(2023, 1, &m, &d) == 0 && m == 1 && d == 1,
          "day 1 is 1 Jan");
    CHECK(month_day(2023, 365, &m, &d) == 0 && m == 12 && d == 31,
          "day 365 of 2023 is 31 Dec");
    CHECK(month_day(2024, 366, &m, &d) == 0 && m == 12 && d == 31,
          "day 366 of 2024 is 31 Dec");
    CHECK(month_day(2024, 60, &m, &d) == 0 && m == 2 && d == 29,
          "day 60 of 2024 is 29 Feb");
    CHECK(month_day(2023, 60, &m, &d) == 0 && m == 3 && d == 1,
          "day 60 of 2023 is 1 Mar");

    /* month_day — out of range leaves the outputs alone */
    m = 42; d = 43;
    CHECK(month_day(2023, 0, &m, &d) == -1 && m == 42 && d == 43,
          "yearday 0 rejected, outputs untouched");
    CHECK(month_day(2023, 366, &m, &d) == -1 && m == 42 && d == 43,
          "day 366 of an ordinary year rejected, outputs untouched");
    CHECK(month_day(2024, 367, &m, &d) == -1 && m == 42 && d == 43,
          "day 367 rejected even in a leap year");

    /* full round trip over an ordinary and a leap year */
    static const int mlen[2][13] = {
        {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    };
    for (int year = 2023; year <= 2024; year++) {
        int leap = is_leap(year);
        int expect = 0, bad = 0;
        for (int mm = 1; mm <= 12; mm++) {
            for (int dd = 1; dd <= mlen[leap][mm]; dd++) {
                expect++;
                int yd = day_of_year(year, mm, dd);
                int m2 = 0, d2 = 0;
                if (yd != expect ||
                    month_day(year, yd, &m2, &d2) != 0 ||
                    m2 != mm || d2 != dd)
                    bad++;
            }
        }
        char msg[64];
        snprintf(msg, sizeof msg, "every day of %d round-trips", year);
        CHECK(bad == 0, msg);
    }

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
