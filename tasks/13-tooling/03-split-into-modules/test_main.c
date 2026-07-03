/* Test harness — provides main() and exercises the stats module through
 * the learner's own stats.h (compiled with -I pointing at WORK_DIR). */
#include <math.h>
#include <stdio.h>

#include "stats.h"

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
    int a[5] = {4, -9, 16, 0, 7};
    CHECK(sum_i(a, 5) == 18, "sum of a small mixed array");
    CHECK(sum_i(a, 1) == 4, "sum of a single element");
    CHECK(sum_i(NULL, 0) == 0, "sum_i(NULL, 0) returns 0");

    CHECK(max_i(a, 5) == 16, "max of a small mixed array");
    int neg[3] = {-8, -2, -30};
    CHECK(max_i(neg, 3) == -2, "max of an all-negative array");
    int one[1] = {42};
    CHECK(max_i(one, 1) == 42, "max of a single element");

    CHECK(fabs(mean_i(a, 5) - 3.6) < 1e-9, "mean of a small mixed array");
    int two[2] = {1, 2};
    CHECK(fabs(mean_i(two, 2) - 1.5) < 1e-9, "non-integer mean is exact");

    /* big enough that an off-by-one read lands outside the buffer */
    enum { N = 512 };
    int big[N];
    long want = 0;
    for (int i = 0; i < N; i++) {
        big[i] = i * 3 - 700;
        want += big[i];
    }
    CHECK(sum_i(big, N) == want, "sum of 512 elements");
    CHECK(max_i(big, N) == 511 * 3 - 700, "max of 512 elements");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
