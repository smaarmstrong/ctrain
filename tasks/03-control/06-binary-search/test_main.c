/* Test harness — provides main() and calls the learner's function.
 * Every array is heap-allocated at exactly n * sizeof(int) so that any
 * out-of-bounds read is an ASan error, not silent luck. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long find_first(const int *a, size_t n, int x);

static int failures;

static void check(const int *src, size_t n, int x, long want, const char *msg)
{
    int *a = NULL;
    if (n > 0) {
        a = malloc(n * sizeof *a);
        if (!a) {
            printf("  FAIL  harness could not allocate\n");
            failures++;
            return;
        }
        memcpy(a, src, n * sizeof *a);
    }
    long got = find_first(a, n, x);
    if (got == want) {
        printf("  ok    %s\n", msg);
    } else {
        printf("  FAIL  %s (expected %ld, got %ld)\n", msg, want, got);
        failures++;
    }
    free(a);
}

int main(void)
{
    check(NULL, 0, 42, -1, "empty array (NULL, n=0) -> -1");

    static const int one[] = {5};
    check(one, 1, 5, 0, "single element, hit");
    check(one, 1, 4, -1, "single element, miss below");
    check(one, 1, 6, -1, "single element, miss above");

    static const int plain[] = {-9, -2, 0, 4, 11, 23, 40};
    check(plain, 7, -9, 0, "target at index 0");
    check(plain, 7, 40, 6, "target at the last index");
    check(plain, 7, 4, 3, "target in the middle");
    check(plain, 7, -100, -1, "smaller than every element -> -1");
    check(plain, 7, 100, -1, "larger than every element -> -1");
    check(plain, 7, 5, -1, "in a gap between elements -> -1");

    static const int dup[] = {1, 3, 3, 3, 5};
    check(dup, 5, 3, 1, "duplicates: first of the 3s");
    check(dup, 5, 1, 0, "duplicates present, unique target at front");
    check(dup, 5, 5, 4, "duplicates present, unique target at back");

    static const int allsame[] = {7, 7, 7};
    check(allsame, 3, 7, 0, "all-equal array returns index 0");
    check(allsame, 3, 8, -1, "all-equal array, miss");

    static const int two[] = {2, 2};
    check(two, 2, 2, 0, "two equal elements -> 0");

    /* a bigger array: evens 0..1998; every odd target is a miss */
    enum { N = 1000 };
    int *big = malloc(N * sizeof *big);
    if (!big) {
        printf("  FAIL  harness could not allocate\n");
        failures++;
    } else {
        for (int i = 0; i < N; i++)
            big[i] = 2 * i;
        int ok = 1;
        for (int i = 0; i < N; i++) {
            if (find_first(big, N, 2 * i) != i)
                ok = 0;
            if (find_first(big, N, 2 * i + 1) != -1)
                ok = 0;
        }
        if (ok) {
            printf("  ok    1000-element array: every hit and every miss\n");
        } else {
            printf("  FAIL  1000-element array: every hit and every miss\n");
            failures++;
        }
        free(big);
    }

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
