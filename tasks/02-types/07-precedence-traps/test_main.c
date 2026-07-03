/* Test harness — provides main() and calls the learner's functions. */
#include <stdbool.h>
#include <stdio.h>

bool between(int lo, int x, int hi);
bool low_nibble_is(unsigned x, unsigned v);
int shifted_sum(int a, int b);
int bonus_total(int base, bool bonus);

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
    /* between */
    CHECK(between(1, 5, 10) == true, "5 is between 1 and 10");
    CHECK(between(5, 1, 10) == false,
          "1 is NOT between 5 and 10 (chained-comparison trap)");
    CHECK(between(1, 11, 10) == false, "11 is not between 1 and 10");
    CHECK(between(3, 3, 3) == true, "bounds are inclusive");
    CHECK(between(-10, -5, -1) == true, "works for negatives");
    CHECK(between(-10, 0, -1) == false, "0 is not between -10 and -1");

    /* low_nibble_is */
    CHECK(low_nibble_is(0x12u, 0x2u) == true, "low nibble of 0x12 is 2");
    CHECK(low_nibble_is(0x12u, 0x3u) == false, "low nibble of 0x12 is not 3");
    CHECK(low_nibble_is(0xF0u, 0x0u) == true, "low nibble of 0xF0 is 0");
    CHECK(low_nibble_is(0xABu, 0xBu) == true, "low nibble of 0xAB is 0xB");
    CHECK(low_nibble_is(0x0u, 0xFu) == false,
          "low nibble of 0 is not 0xF (== vs & trap)");

    /* shifted_sum */
    CHECK(shifted_sum(1, 0) == 4, "(1 << 2) + 0 == 4");
    CHECK(shifted_sum(1, 3) == 7, "(1 << 2) + 3 == 7 (shift vs + trap)");
    CHECK(shifted_sum(0, 9) == 9, "(0 << 2) + 9 == 9");
    CHECK(shifted_sum(10, 1) == 41, "(10 << 2) + 1 == 41");
    CHECK(shifted_sum(5, 5) == 25, "(5 << 2) + 5 == 25");

    /* bonus_total */
    CHECK(bonus_total(5, false) == 5, "no bonus leaves base alone");
    CHECK(bonus_total(5, true) == 15, "bonus adds 10");
    CHECK(bonus_total(0, false) == 0, "zero base, no bonus");
    CHECK(bonus_total(0, true) == 10, "zero base, bonus");
    CHECK(bonus_total(100, false) == 100,
          "large base unchanged (ternary-binding trap)");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
