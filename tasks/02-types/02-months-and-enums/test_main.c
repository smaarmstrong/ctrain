/* Test harness — provides main() and calls the learner's functions. */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

const char *month_name(int m);
int days_in_month(int m, bool leap);
int day_of_year(int m, int d, bool leap);

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

static bool name_is(int m, const char *want)
{
    const char *got = month_name(m);
    return got != NULL && strcmp(got, want) == 0;
}

int main(void)
{
    /* month_name */
    CHECK(name_is(1, "January"), "month_name(1) is \"January\"");
    CHECK(name_is(2, "February"), "month_name(2) is \"February\"");
    CHECK(name_is(9, "September"), "month_name(9) is \"September\"");
    CHECK(name_is(12, "December"), "month_name(12) is \"December\"");
    CHECK(name_is(0, "?"), "month_name(0) is \"?\"");
    CHECK(name_is(13, "?"), "month_name(13) is \"?\"");
    CHECK(name_is(-4, "?"), "month_name(-4) is \"?\"");

    /* the pointer must survive later calls (no local buffers) */
    const char *jan = month_name(1);
    (void)month_name(7);
    (void)month_name(0);
    CHECK(strcmp(jan, "January") == 0,
          "month_name result still valid after further calls");

    /* days_in_month */
    CHECK(days_in_month(1, false) == 31, "January has 31 days");
    CHECK(days_in_month(2, false) == 28, "February has 28 days (common year)");
    CHECK(days_in_month(2, true) == 29, "February has 29 days (leap year)");
    CHECK(days_in_month(4, false) == 30, "April has 30 days");
    CHECK(days_in_month(4, true) == 30, "leap only changes February");
    CHECK(days_in_month(12, true) == 31, "December has 31 days");
    CHECK(days_in_month(0, false) == 0, "days_in_month(0, _) is 0");
    CHECK(days_in_month(13, true) == 0, "days_in_month(13, _) is 0");

    /* day_of_year */
    CHECK(day_of_year(1, 1, false) == 1, "Jan 1 is day 1");
    CHECK(day_of_year(1, 31, false) == 31, "Jan 31 is day 31");
    CHECK(day_of_year(2, 28, false) == 59, "Feb 28 is day 59 (common)");
    CHECK(day_of_year(3, 1, false) == 60, "Mar 1 is day 60 (common)");
    CHECK(day_of_year(3, 1, true) == 61, "Mar 1 is day 61 (leap)");
    CHECK(day_of_year(12, 31, false) == 365, "Dec 31 is day 365 (common)");
    CHECK(day_of_year(12, 31, true) == 366, "Dec 31 is day 366 (leap)");
    CHECK(day_of_year(2, 29, false) == -1, "Feb 29 invalid in a common year");
    CHECK(day_of_year(2, 29, true) == 60, "Feb 29 is day 60 in a leap year");
    CHECK(day_of_year(4, 31, false) == -1, "Apr 31 is invalid");
    CHECK(day_of_year(0, 1, false) == -1, "month 0 is invalid");
    CHECK(day_of_year(13, 1, false) == -1, "month 13 is invalid");
    CHECK(day_of_year(6, 0, false) == -1, "day 0 is invalid");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
