/* Test harness — provides main() and calls the learner's functions. */
#include <stdio.h>

struct stats {
    int  count;
    long sum;
    int  min;
    int  max;
};

void stats_init(struct stats *s);
void stats_add(struct stats *s, int value);
double stats_mean(const struct stats *s);
void stats_merge(struct stats *dst, const struct stats *src);

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
    struct stats a = {99, 99, 99, 99};
    stats_init(&a);
    CHECK(a.count == 0 && a.sum == 0, "stats_init zeroes count and sum");
    CHECK(stats_mean(&a) == 0.0, "mean of an empty stats is 0.0");

    stats_add(&a, 5);
    CHECK(a.count == 1 && a.sum == 5 && a.min == 5 && a.max == 5,
          "first add sets count, sum, min and max");

    stats_add(&a, -2);
    stats_add(&a, 9);
    CHECK(a.count == 3 && a.sum == 12, "three adds accumulate count and sum");
    CHECK(a.min == -2 && a.max == 9, "min and max track extremes");
    CHECK(stats_mean(&a) == 4.0, "mean of 5, -2, 9 is exactly 4.0");

    struct stats b;
    stats_init(&b);
    stats_add(&b, -10);
    stats_add(&b, 20);

    stats_merge(&a, &b);
    CHECK(a.count == 5 && a.sum == 22, "merge combines counts and sums");
    CHECK(a.min == -10 && a.max == 20, "merge combines min and max");
    CHECK(b.count == 2 && b.sum == 10 && b.min == -10 && b.max == 20,
          "merge leaves src untouched");

    struct stats empty;
    stats_init(&empty);
    stats_merge(&a, &empty);
    CHECK(a.count == 5 && a.sum == 22 && a.min == -10 && a.max == 20,
          "merging an empty src changes nothing");

    struct stats fresh;
    stats_init(&fresh);
    stats_merge(&fresh, &b);
    CHECK(fresh.count == 2 && fresh.sum == 10 && fresh.min == -10 && fresh.max == 20,
          "merging into an empty dst copies src's stats");
    CHECK(stats_mean(&fresh) == 5.0, "mean of the merged copy is 5.0");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
