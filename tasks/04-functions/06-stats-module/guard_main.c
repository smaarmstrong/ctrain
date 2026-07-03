/* Grader harness. The learner's header is included twice on purpose:
 * without an include guard, struct stats gets defined twice and this
 * file does not compile. */
#include "stats.h"
#include "stats.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>

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
    static const long vals[] = {5, -3, 12, 0, 6};
    struct stats s = stats_of(vals, 5);
    CHECK(s.count == 5, "count of {5,-3,12,0,6} is 5");
    CHECK(s.min == -3, "min is -3");
    CHECK(s.max == 12, "max is 12");
    CHECK(s.sum == 20, "sum is 20");
    CHECK(fabs(s.mean - 4.0) < 1e-9, "mean is 4.0");

    static const long one[] = {-8};
    s = stats_of(one, 1);
    CHECK(s.count == 1 && s.min == -8 && s.max == -8 && s.sum == -8,
          "a single value is its own min, max and sum");
    CHECK(fabs(s.mean + 8.0) < 1e-9, "mean of a single -8 is -8.0");

    s = stats_of(NULL, 0);
    CHECK(s.count == 0 && s.min == 0 && s.max == 0 && s.sum == 0,
          "stats_of(NULL, 0) zeroes count, min, max and sum");
    CHECK(s.mean == 0.0, "stats_of(NULL, 0) zeroes the mean");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
