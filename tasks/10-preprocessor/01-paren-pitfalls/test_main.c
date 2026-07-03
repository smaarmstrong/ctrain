/* Test harness — textually includes the learner's macros, then expands
 * them inside expressions that punish missing parentheses. */
#include <stdio.h>

#include "main.c"

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
    /* plain values first */
    CHECK(SQ(4) == 16, "SQ(4) == 16");
    CHECK(MIN(2, 3) == 2 && MIN(3, 2) == 2, "MIN picks the smaller value");
    CHECK(MAX(2, 3) == 3 && MAX(3, 2) == 3, "MAX picks the larger value");
    CHECK(CLAMP(5, 0, 10) == 5, "CLAMP leaves in-range values alone");
    CHECK(CLAMP(-3, 0, 10) == 0, "CLAMP raises low values to lo");
    CHECK(CLAMP(42, 0, 10) == 10, "CLAMP lowers high values to hi");

    /* expressions as arguments */
    CHECK(SQ(1 + 2) == 9, "SQ(1 + 2) == 9");
    CHECK(CLAMP(2 + 2, 0, 3) == 3, "CLAMP(2 + 2, 0, 3) == 3");
    int a = 6, b = 4;
    CHECK(MIN(a + 1, b + 4) == 7, "MIN(a + 1, b + 4) == 7");

    /* expansions embedded in larger expressions */
    CHECK(72 / SQ(2 + 1) == 8, "72 / SQ(2 + 1) == 8");
    int neg = -SQ(1 + 1);
    CHECK(neg == -4, "-SQ(1 + 1) == -4");
    CHECK(MIN(2, 3) * 10 == 20, "MIN(2, 3) * 10 == 20");
    CHECK(1 + MIN(2, 3) == 3, "1 + MIN(2, 3) == 3");
    CHECK(10 - MAX(1, 5) == 5, "10 - MAX(1, 5) == 5");
    CHECK(100 / CLAMP(3, 1, 4) == 33, "100 / CLAMP(3, 1, 4) == 33");

    /* nested expansions */
    CHECK(MAX(MIN(9, 5), SQ(2)) == 5, "MAX(MIN(9, 5), SQ(2)) == 5");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
