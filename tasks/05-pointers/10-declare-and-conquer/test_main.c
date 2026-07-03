/* Test harness — provides main(); the learner's file defines these. */
#include <stddef.h>
#include <stdio.h>

extern int (*ops[4])(int, int);
int (*get_op(char op))(int, int);
int (*pick_row(int (*m)[4], size_t nrows, size_t r))[4];

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
    /* 1. the operation table */
    int filled = ops[0] && ops[1] && ops[2] && ops[3];
    CHECK(filled, "all four ops slots are filled");
    if (filled) {
        CHECK(ops[0](2, 3) == 5, "ops[0] adds");
        CHECK(ops[1](2, 3) == -1, "ops[1] subtracts");
        CHECK(ops[2](4, 5) == 20, "ops[2] multiplies");
        CHECK(ops[3](17, 5) == 3, "ops[3] divides (integer division)");
        CHECK(ops[3](-9, 2) == -4, "integer division truncates toward zero");
    } else {
        failures += 5;
    }

    /* 2. get_op returns the very same pointers */
    CHECK(get_op('+') == ops[0], "get_op('+') is ops[0] itself");
    CHECK(get_op('-') == ops[1], "get_op('-') is ops[1] itself");
    CHECK(get_op('*') == ops[2], "get_op('*') is ops[2] itself");
    CHECK(get_op('/') == ops[3], "get_op('/') is ops[3] itself");
    CHECK(get_op('%') == NULL, "get_op('%') is NULL");
    CHECK(get_op('x') == NULL, "get_op('x') is NULL");
    CHECK(get_op('\0') == NULL, "get_op('\\0') is NULL");

    int (*mul)(int, int) = get_op('*');
    CHECK(mul != NULL && mul(6, 7) == 42, "a fetched pointer is callable");

    /* drive a tiny calculation entirely through the table */
    if (filled) {
        int acc = 1;
        const char script[] = "+*-";
        const int arg[] = {9, 4, 15};
        int ok = 1;
        for (int i = 0; i < 3; i++) {
            int (*f)(int, int) = get_op(script[i]);
            if (!f) { ok = 0; break; }
            acc = f(acc, arg[i]);
        }
        /* ((1+9)*4)-15 = 25 */
        CHECK(ok && acc == 25, "chained dispatch: ((1+9)*4)-15 == 25");
    } else {
        failures += 1;
    }

    /* 3. pick_row */
    int m[3][4] = {
        {0, 1, 2, 3},
        {10, 11, 12, 13},
        {20, 21, 22, 23},
    };
    CHECK(pick_row(m, 3, 0) == &m[0], "row 0 is &m[0]");
    CHECK(pick_row(m, 3, 2) == &m[2], "row 2 is &m[2]");
    int (*row)[4] = pick_row(m, 3, 1);
    CHECK(row != NULL && (*row)[0] == 10 && (*row)[3] == 13,
          "the returned pointer reaches the row's elements");
    if (row) {
        (*row)[2] = 99;
        CHECK(m[1][2] == 99, "writing through the row pointer hits the matrix");
    } else {
        failures += 1;
    }
    CHECK(pick_row(m, 3, 3) == NULL, "r == nrows is out of range");
    CHECK(pick_row(m, 3, 999) == NULL, "far out of range is NULL too");
    CHECK(pick_row(m, 1, 0) == &m[0] && pick_row(m, 1, 1) == NULL,
          "nrows is respected, not the real array size");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
