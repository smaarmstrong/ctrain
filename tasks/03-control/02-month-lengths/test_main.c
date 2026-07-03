/* Test harness — provides main() and calls the learner's functions. */
#include <stdio.h>

int is_leap(int year);
int days_in_month(int month, int year);

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

int main(void)
{
    /* is_leap on its own */
    CHECK(is_leap(2024) == 1, "2024 is a leap year");
    CHECK(is_leap(2023) == 0, "2023 is not a leap year");
    CHECK(is_leap(1996) == 1, "1996 is a leap year");
    CHECK(is_leap(1900) == 0, "1900 (century) is not a leap year");
    CHECK(is_leap(2000) == 1, "2000 (4th century) is a leap year");
    CHECK(is_leap(2100) == 0, "2100 (century) is not a leap year");

    /* every 31-day month — a fallthrough slip shows up here */
    static const int m31[] = {1, 3, 5, 7, 8, 10, 12};
    int all31 = 1;
    for (int i = 0; i < 7; i++)
        if (days_in_month(m31[i], 2023) != 31)
            all31 = 0;
    CHECK(all31, "months 1,3,5,7,8,10,12 have 31 days");

    /* every 30-day month */
    static const int m30[] = {4, 6, 9, 11};
    int all30 = 1;
    for (int i = 0; i < 4; i++)
        if (days_in_month(m30[i], 2023) != 30)
            all30 = 0;
    CHECK(all30, "months 4,6,9,11 have 30 days");

    /* February */
    CHECK(days_in_month(2, 2023) == 28, "February 2023 has 28 days");
    CHECK(days_in_month(2, 2024) == 29, "February 2024 has 29 days");
    CHECK(days_in_month(2, 1900) == 28, "February 1900 has 28 days");
    CHECK(days_in_month(2, 2000) == 29, "February 2000 has 29 days");

    /* invalid months */
    CHECK(days_in_month(0, 2024) == -1, "month 0 is invalid");
    CHECK(days_in_month(13, 2024) == -1, "month 13 is invalid");
    CHECK(days_in_month(-2, 2024) == -1, "month -2 is invalid");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
