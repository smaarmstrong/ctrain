/* Test harness — provides main() and calls the learner's function.
 * The output buffer is heap-allocated at exactly 12 bytes so that any
 * write past the end is an ASan error, not silent corruption. */
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void int_to_decimal(int n, char out[]);

static int failures;

static void check(int n, const char *want)
{
    char *buf = malloc(12);
    if (!buf) {
        printf("  FAIL  harness could not allocate\n");
        failures++;
        return;
    }
    memset(buf, '#', 12); /* no free NUL terminator lying around */
    int_to_decimal(n, buf);
    if (memchr(buf, '\0', 12) != NULL && strcmp(buf, want) == 0) {
        printf("  ok    %d -> \"%s\"\n", n, want);
    } else {
        printf("  FAIL  %d -> expected \"%s\", got \"%.12s\"\n", n, want, buf);
        failures++;
    }
    free(buf);
}

int main(void)
{
    check(0, "0");
    check(7, "7");
    check(42, "42");
    check(-5, "-5");
    check(100, "100");
    check(-1000000, "-1000000");
    check(12345, "12345");
    check(INT_MAX, "2147483647");
    check(INT_MIN, "-2147483648");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
