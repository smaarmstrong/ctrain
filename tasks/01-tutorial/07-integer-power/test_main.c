/* Test harness — provides main() and calls the learner's int_pow(). */
#include <stdio.h>

long int_pow(int base, int exp);

static int failures;

static void check(long got, long want, const char *msg)
{
    if (got == want) {
        printf("  ok    %s\n", msg);
    } else {
        printf("  FAIL  %s (want %ld, got %ld)\n", msg, want, got);
        failures++;
    }
}

int main(void)
{
    check(int_pow(2, 10), 1024L, "2^10");
    check(int_pow(10, 3), 1000L, "10^3");
    check(int_pow(5, 1), 5L, "5^1");
    check(int_pow(3, 0), 1L, "3^0 is 1");
    check(int_pow(0, 0), 1L, "0^0 is defined as 1");
    check(int_pow(0, 7), 0L, "0^7 is 0");
    check(int_pow(1, 62), 1L, "1^62");
    check(int_pow(-1, 61), -1L, "-1^61 (odd exponent)");
    check(int_pow(-2, 3), -8L, "-2^3 is -8");
    check(int_pow(-2, 8), 256L, "-2^8 is 256");
    check(int_pow(7, 11), 1977326743L, "7^11");
    check(int_pow(2, 62), 4611686018427387904L, "2^62 needs a long accumulator");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
