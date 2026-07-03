/* Test harness — provides main() and calls the learner's functions.
 *
 * These globals exist on purpose: they collide, at link time, with
 * any external-linkage state in the learner's file that uses the
 * same names. Private module state must be `static`. */
#include <stddef.h>
#include <stdio.h>

long count = 111;
long sum = 222;
long total = 333;
long min = 444;
long max = 555;

void tally_reset(void);
void tally_add(long v);
size_t tally_count(void);
long tally_sum(void);
long tally_min(void);
long tally_max(void);

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
    /* fresh module, no reset called yet */
    CHECK(tally_count() == 0, "starts with count 0");
    CHECK(tally_sum() == 0, "starts with sum 0");
    CHECK(tally_min() == 0, "starts with min 0");
    CHECK(tally_max() == 0, "starts with max 0");

    tally_add(5);
    tally_add(-3);
    tally_add(9);
    CHECK(tally_count() == 3, "count is 3 after three adds");
    CHECK(tally_sum() == 11, "sum of 5, -3, 9 is 11");
    CHECK(tally_min() == -3, "min of 5, -3, 9 is -3");
    CHECK(tally_max() == 9, "max of 5, -3, 9 is 9");

    tally_reset();
    CHECK(tally_count() == 0, "reset clears the count");
    CHECK(tally_sum() == 0, "reset clears the sum");
    CHECK(tally_min() == 0, "reset clears the min");
    CHECK(tally_max() == 0, "reset clears the max");

    tally_add(-7);
    CHECK(tally_count() == 1, "count is 1 after one add");
    CHECK(tally_sum() == -7, "sum of a single -7 is -7");
    CHECK(tally_min() == -7, "min of a single -7 is -7");
    CHECK(tally_max() == -7, "max of a single -7 is -7");

    tally_add(0);
    CHECK(tally_min() == -7 && tally_max() == 0,
          "min/max correct after adding 0 to {-7}");

    CHECK(count == 111 && sum == 222 && total == 333 &&
          min == 444 && max == 555,
          "the harness's own globals are untouched");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
