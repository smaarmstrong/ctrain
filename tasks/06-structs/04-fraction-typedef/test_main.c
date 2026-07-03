/* Test harness — provides main() and calls the learner's functions. */
#include <stdio.h>

typedef struct {
    long num;
    long den;
} Frac;

Frac frac_make(long num, long den);
Frac frac_add(Frac a, Frac b);
Frac frac_mul(Frac a, Frac b);
int frac_cmp(Frac a, Frac b);

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

static int is(Frac f, long num, long den)
{
    return f.num == num && f.den == den;
}

int main(void)
{
    CHECK(is(frac_make(3, 4), 3, 4), "frac_make keeps 3/4 as 3/4");
    CHECK(is(frac_make(2, 4), 1, 2), "frac_make reduces 2/4 to 1/2");
    CHECK(is(frac_make(-2, 4), -1, 2), "frac_make(-2, 4) is -1/2");
    CHECK(is(frac_make(2, -4), -1, 2), "frac_make(2, -4) is -1/2");
    CHECK(is(frac_make(-2, -4), 1, 2), "frac_make(-2, -4) is 1/2");
    CHECK(is(frac_make(0, -7), 0, 1), "zero is always 0/1");
    CHECK(is(frac_make(84, 36), 7, 3), "frac_make reduces 84/36 to 7/3");

    Frac sixth = frac_make(1, 6), third = frac_make(1, 3), half = frac_make(1, 2);
    CHECK(is(frac_add(sixth, third), 1, 2), "1/6 + 1/3 = 1/2, reduced");
    CHECK(is(frac_add(half, frac_make(-1, 2)), 0, 1), "1/2 + (-1/2) = 0/1");
    CHECK(is(frac_add(frac_make(-3, 4), frac_make(1, 4)), -1, 2),
          "-3/4 + 1/4 = -1/2");

    CHECK(is(frac_mul(frac_make(2, 3), frac_make(3, 4)), 1, 2),
          "2/3 * 3/4 = 1/2, reduced");
    CHECK(is(frac_mul(frac_make(0, 5), frac_make(5, 7)), 0, 1),
          "0 * 5/7 = 0/1");
    CHECK(is(frac_mul(frac_make(-1, 2), frac_make(-2, 3)), 1, 3),
          "-1/2 * -2/3 = 1/3");

    CHECK(frac_cmp(third, half) == -1, "1/3 < 1/2");
    CHECK(frac_cmp(half, third) == 1, "1/2 > 1/3");
    CHECK(frac_cmp(frac_make(3, 6), half) == 0, "3/6 == 1/2");
    CHECK(frac_cmp(frac_make(-1, 2), frac_make(-1, 3)) == -1, "-1/2 < -1/3");
    CHECK(frac_cmp(frac_make(0, 3), frac_make(0, 9)) == 0, "0 == 0");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
