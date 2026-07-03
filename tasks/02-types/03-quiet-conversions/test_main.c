/* Test harness — provides main() and calls the learner's functions. */
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>

double mean_of(const int *a, size_t n);
int percent_of(int part, int whole);
unsigned char low_byte(long v);
int clamp_to_int(double d);

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

static int close_to(double got, double want)
{
    return fabs(got - want) < 1e-9;
}

int main(void)
{
    /* mean_of — fractions, empties, sums beyond int */
    int small[4] = {1, 2, 3, 5};
    CHECK(close_to(mean_of(small, 4), 2.75), "mean of {1,2,3,5} is 2.75");
    CHECK(close_to(mean_of(small, 1), 1.0), "mean of a single element");
    CHECK(close_to(mean_of(NULL, 0), 0.0), "mean of nothing is 0.0");
    int big[3] = {INT_MAX, INT_MAX, INT_MAX};
    CHECK(close_to(mean_of(big, 3), (double)INT_MAX),
          "mean of {INT_MAX x3} — the sum must not overflow int");
    int mixed[2] = {INT_MIN, INT_MAX};
    CHECK(close_to(mean_of(mixed, 2), -0.5),
          "mean of {INT_MIN, INT_MAX} is -0.5 (not 0)");

    /* percent_of — exact, truncated, overflow-safe */
    CHECK(percent_of(1, 4) == 25, "1 of 4 is 25%");
    CHECK(percent_of(2, 3) == 66, "2 of 3 truncates to 66%");
    CHECK(percent_of(0, 7) == 0, "0 of 7 is 0%");
    CHECK(percent_of(7, 7) == 100, "7 of 7 is 100%");
    CHECK(percent_of(50000000, 200000000) == 25,
          "50M of 200M is 25% — part*100 must not overflow");
    CHECK(percent_of(2000000000, 2000000000) == 100,
          "2G of 2G is 100% — needs 64-bit intermediate");
    CHECK(percent_of(-1, 3) == -33, "-1 of 3 truncates toward zero (-33)");

    /* low_byte */
    CHECK(low_byte(0) == 0, "low_byte(0)");
    CHECK(low_byte(0x1234) == 0x34, "low_byte(0x1234) is 0x34");
    CHECK(low_byte(255) == 255, "low_byte(255)");
    CHECK(low_byte(256) == 0, "low_byte(256) is 0");
    CHECK(low_byte(-1) == 255, "low_byte(-1) is 255");
    CHECK(low_byte(-256) == 0, "low_byte(-256) is 0");
    CHECK(low_byte(-2) == 254, "low_byte(-2) is 254");

    /* clamp_to_int */
    CHECK(clamp_to_int(0.0) == 0, "clamp_to_int(0.0)");
    CHECK(clamp_to_int(3.9) == 3, "3.9 truncates to 3");
    CHECK(clamp_to_int(-3.9) == -3, "-3.9 truncates to -3");
    CHECK(clamp_to_int(2147483647.0) == INT_MAX, "INT_MAX exactly fits");
    CHECK(clamp_to_int(-2147483648.0) == INT_MIN, "INT_MIN exactly fits");
    CHECK(clamp_to_int(1e18) == INT_MAX, "1e18 saturates to INT_MAX");
    CHECK(clamp_to_int(-1e18) == INT_MIN, "-1e18 saturates to INT_MIN");
    CHECK(clamp_to_int(2147483648.0) == INT_MAX,
          "INT_MAX + 1 saturates (no UB cast)");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
