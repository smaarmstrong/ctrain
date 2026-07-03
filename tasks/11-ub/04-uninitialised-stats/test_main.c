/* Test harness — provides main() and calls the learner's functions.
 * scribble() fills the stack region the callee's frame will occupy with
 * 0xAA bytes, so an uninitialised local cannot luckily be zero. */
#include <stddef.h>
#include <stdio.h>

int max_of(const int *a, size_t n);
long long sum_of(const int *a, size_t n);

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

static void __attribute__((noinline)) scribble(void)
{
    volatile unsigned char junk[1024];
    for (size_t i = 0; i < sizeof junk; i++)
        junk[i] = 0xAA;
}

int main(void)
{
    const int mixed[3] = {9, 2, 5};
    const int single[1] = {-7};
    const int negs[4] = {-31, -7, -50, -12};
    const int big[3] = {1000000000, 1000000000, 1000000000};

    scribble();
    int m1 = max_of(mixed, 3);
    CHECK(m1 == 9, "max_of counts the first element too");

    scribble();
    int m2 = max_of(single, 1);
    CHECK(m2 == -7, "max_of of a single element");

    scribble();
    int m3 = max_of(negs, 4);
    CHECK(m3 == -7, "max_of of an all-negative array");

    scribble();
    long long s1 = sum_of(mixed, 3);
    CHECK(s1 == 16, "sum_of a small array");

    scribble();
    long long s0 = sum_of(mixed, 0);
    CHECK(s0 == 0, "sum_of nothing is 0");

    scribble();
    long long s2 = sum_of(big, 3);
    CHECK(s2 == 3000000000LL, "sum_of does not overflow an int");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
