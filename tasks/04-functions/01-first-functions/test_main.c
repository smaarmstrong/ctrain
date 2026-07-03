/* Test harness — provides main() and calls the learner's functions. */
#include <stdio.h>

long ipow(int base, int exp);
int clampi(int x, int lo, int hi);
int digit_count(long n);

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
    CHECK(ipow(2, 10) == 1024L, "ipow(2, 10) == 1024");
    CHECK(ipow(-3, 3) == -27L, "ipow(-3, 3) == -27");
    CHECK(ipow(-3, 4) == 81L, "ipow(-3, 4) == 81");
    CHECK(ipow(7, 0) == 1L, "ipow(7, 0) == 1");
    CHECK(ipow(0, 0) == 1L, "ipow(0, 0) == 1");
    CHECK(ipow(0, 5) == 0L, "ipow(0, 5) == 0");
    CHECK(ipow(1, 31) == 1L, "ipow(1, 31) == 1");
    CHECK(ipow(10, 9) == 1000000000L, "ipow(10, 9) == 1000000000");

    CHECK(clampi(5, 1, 10) == 5, "clampi leaves an in-range value alone");
    CHECK(clampi(-4, 1, 10) == 1, "clampi raises a low value to lo");
    CHECK(clampi(99, 1, 10) == 10, "clampi lowers a high value to hi");
    CHECK(clampi(1, 1, 10) == 1, "clampi keeps a value equal to lo");
    CHECK(clampi(10, 1, 10) == 10, "clampi keeps a value equal to hi");
    CHECK(clampi(3, 3, 3) == 3, "clampi with lo == hi");
    CHECK(clampi(-7, -9, -5) == -7, "clampi with a negative range");

    CHECK(digit_count(0) == 1, "digit_count(0) == 1");
    CHECK(digit_count(7) == 1, "digit_count(7) == 1");
    CHECK(digit_count(-7) == 1, "digit_count(-7) == 1");
    CHECK(digit_count(10) == 2, "digit_count(10) == 2");
    CHECK(digit_count(-12345) == 5, "digit_count(-12345) == 5");
    CHECK(digit_count(1000000000L) == 10, "digit_count(1000000000) == 10");
    CHECK(digit_count(999999999L) == 9, "digit_count(999999999) == 9");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
