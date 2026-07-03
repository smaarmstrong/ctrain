/* Test harness — provides main() and calls the learner's functions. */
#include <stddef.h>
#include <stdio.h>

long sum_range(const int *begin, const int *end);
const int *find_first(const int *begin, const int *end, int x);
size_t count_above(const int *begin, const int *end, int x);

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
    int a[8] = {4, -2, 9, 4, 0, 7, 4, -5};

    /* sum_range */
    CHECK(sum_range(a, a + 8) == 21, "sum of the whole array");
    CHECK(sum_range(a, a) == 0, "empty range sums to 0");
    CHECK(sum_range(a + 2, a + 5) == 13, "sum of an interior sub-range");
    CHECK(sum_range(a + 7, a + 8) == -5, "single-element range");

    /* find_first */
    CHECK(find_first(a, a + 8, 4) == a, "first match at the front");
    CHECK(find_first(a, a + 8, 7) == a + 5, "match in the middle");
    CHECK(find_first(a, a + 8, 100) == a + 8, "no match returns end");
    CHECK(find_first(a + 3, a + 8, 4) == a + 3,
          "search starts at begin, not at the array");
    CHECK(find_first(a, a, 4) == a, "empty range returns end (== begin)");

    /* count_above */
    CHECK(count_above(a, a + 8, 3) == 5, "five elements above 3");
    CHECK(count_above(a, a + 8, 8) == 1, "one element above 8");
    CHECK(count_above(a, a + 8, 100) == 0, "none above 100");
    CHECK(count_above(a, a + 8, -100) == 8, "all above -100");
    CHECK(count_above(a, a, 0) == 0, "empty range counts 0");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
