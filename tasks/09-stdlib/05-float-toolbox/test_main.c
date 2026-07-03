/* Test harness — provides main() and calls the learner's functions. */
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

const char *fp_class(double x);
long long ll_round(double x);
double round_places(double x, int places);

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

static int approx(double a, double b)
{
    return fabs(a - b) < 1e-9;
}

static int classed(double x, const char *want)
{
    const char *got = fp_class(x);
    return got != NULL && strcmp(got, want) == 0;
}

int main(void)
{
    /* fp_class */
    CHECK(classed(nan(""), "nan"), "NaN -> \"nan\"");
    CHECK(classed(INFINITY, "+inf"), "INFINITY -> \"+inf\"");
    CHECK(classed(-INFINITY, "-inf"), "-INFINITY -> \"-inf\"");
    CHECK(classed(0.0, "zero"), "0.0 -> \"zero\"");
    CHECK(classed(-0.0, "zero"), "-0.0 -> \"zero\"");
    CHECK(classed(5e-324, "subnormal"), "5e-324 -> \"subnormal\"");
    CHECK(classed(DBL_MIN / 4.0, "subnormal"), "DBL_MIN/4 -> \"subnormal\"");
    CHECK(classed(1.0, "normal"), "1.0 -> \"normal\"");
    CHECK(classed(-DBL_MAX, "normal"), "-DBL_MAX -> \"normal\"");
    CHECK(classed(DBL_MIN, "normal"), "DBL_MIN itself is still \"normal\"");

    /* ll_round */
    CHECK(ll_round(2.5) == 3, "2.5 -> 3 (half away from zero)");
    CHECK(ll_round(-2.5) == -3, "-2.5 -> -3 (half away from zero)");
    CHECK(ll_round(2.4) == 2, "2.4 -> 2");
    CHECK(ll_round(-2.6) == -3, "-2.6 -> -3");
    CHECK(ll_round(0.0) == 0, "0.0 -> 0");
    CHECK(ll_round(1e15 + 0.5) == 1000000000000001LL, "large values survive");

    /* round_places */
    CHECK(approx(round_places(1.25, 1), 1.3), "1.25 @1 -> 1.3");
    CHECK(approx(round_places(-1.25, 1), -1.3), "-1.25 @1 -> -1.3");
    CHECK(approx(round_places(3.14159, 2), 3.14), "3.14159 @2 -> 3.14");
    CHECK(approx(round_places(3.14159, 4), 3.1416), "3.14159 @4 -> 3.1416");
    CHECK(approx(round_places(2.5, 0), 3.0), "2.5 @0 -> 3.0");
    CHECK(approx(round_places(-7.0, 3), -7.0), "integers pass through");
    CHECK(approx(round_places(0.000123456, 6), 0.000123), "small magnitudes");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
