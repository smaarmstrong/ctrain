/* Test harness — provides main() and calls the learner's functions. */
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>

bool add_ok(int a, int b, int *result);
bool sub_ok(int a, int b, int *result);
bool mul_ok(int a, int b, int *result);

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

#define SENTINEL 123454321

/* ok_case: OP must succeed and produce `want`. */
static void ok_case(bool (*op)(int, int, int *), int a, int b, int want,
                    const char *msg)
{
    int r = SENTINEL;
    bool ret = op(a, b, &r);
    CHECK(ret && r == want, msg);
}

/* over_case: OP must report overflow and leave *result untouched. */
static void over_case(bool (*op)(int, int, int *), int a, int b,
                      const char *msg)
{
    int r = SENTINEL;
    bool ret = op(a, b, &r);
    CHECK(!ret && r == SENTINEL, msg);
}

int main(void)
{
    /* --- add_ok --- */
    ok_case(add_ok, 2, 3, 5, "add_ok(2, 3) -> 5");
    ok_case(add_ok, -2, -3, -5, "add_ok(-2, -3) -> -5");
    ok_case(add_ok, INT_MAX, 0, INT_MAX, "add_ok(INT_MAX, 0) fits");
    ok_case(add_ok, INT_MAX, INT_MIN, -1, "add_ok(INT_MAX, INT_MIN) -> -1");
    ok_case(add_ok, INT_MIN + 1, -1, INT_MIN, "add_ok lands exactly on INT_MIN");
    over_case(add_ok, INT_MAX, 1, "add_ok(INT_MAX, 1) overflows");
    over_case(add_ok, 1, INT_MAX, "add_ok(1, INT_MAX) overflows");
    over_case(add_ok, INT_MIN, -1, "add_ok(INT_MIN, -1) overflows");
    over_case(add_ok, INT_MIN / 2 - 1, INT_MIN / 2 - 1, "add_ok deep negative overflow");

    /* --- sub_ok --- */
    ok_case(sub_ok, 10, 3, 7, "sub_ok(10, 3) -> 7");
    ok_case(sub_ok, -10, -3, -7, "sub_ok(-10, -3) -> -7");
    ok_case(sub_ok, INT_MIN, 0, INT_MIN, "sub_ok(INT_MIN, 0) fits");
    ok_case(sub_ok, INT_MAX, INT_MAX, 0, "sub_ok(INT_MAX, INT_MAX) -> 0");
    ok_case(sub_ok, -1, INT_MAX, INT_MIN, "sub_ok lands exactly on INT_MIN");
    over_case(sub_ok, INT_MIN, 1, "sub_ok(INT_MIN, 1) overflows");
    over_case(sub_ok, 0, INT_MIN, "sub_ok(0, INT_MIN) overflows");
    over_case(sub_ok, INT_MAX, -1, "sub_ok(INT_MAX, -1) overflows");
    over_case(sub_ok, -2, INT_MAX, "sub_ok(-2, INT_MAX) overflows");

    /* --- mul_ok --- */
    ok_case(mul_ok, 6, 7, 42, "mul_ok(6, 7) -> 42");
    ok_case(mul_ok, -6, 7, -42, "mul_ok(-6, 7) -> -42");
    ok_case(mul_ok, 0, INT_MIN, 0, "mul_ok(0, INT_MIN) -> 0");
    ok_case(mul_ok, INT_MIN, 1, INT_MIN, "mul_ok(INT_MIN, 1) fits");
    ok_case(mul_ok, 46340, 46340, 2147395600, "mul_ok just under the limit");
    ok_case(mul_ok, -46341, 46340, -2147441940,
            "mul_ok large negative product that still fits");
    over_case(mul_ok, 46341, 46341, "mul_ok just over INT_MAX overflows");
    over_case(mul_ok, -46341, 46341, "mul_ok just under INT_MIN overflows");
    over_case(mul_ok, INT_MIN, -1, "mul_ok(INT_MIN, -1) overflows");
    over_case(mul_ok, INT_MAX, 2, "mul_ok(INT_MAX, 2) overflows");
    over_case(mul_ok, 2, INT_MIN, "mul_ok(2, INT_MIN) overflows");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
