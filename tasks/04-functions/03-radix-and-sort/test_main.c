/* Test harness — provides main() and calls the learner's functions. */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fmt_base(unsigned long n, unsigned base, char *out);
void sort_ints(int *a, size_t n);

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

static void check_fmt(unsigned long n, unsigned base, const char *want)
{
    char buf[80];
    char msg[128];
    memset(buf, '#', sizeof buf);
    fmt_base(n, base, buf);
    snprintf(msg, sizeof msg, "fmt_base(%lu, %u) == \"%s\"", n, base, want);
    CHECK(strcmp(buf, want) == 0, msg);
}

static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

static void check_sort(const int *src, size_t n, const char *msg)
{
    int *mine = NULL, *ref = NULL;
    if (n > 0) {
        mine = malloc(n * sizeof *mine);
        ref = malloc(n * sizeof *ref);
        if (!mine || !ref) {
            fprintf(stderr, "harness: out of memory\n");
            exit(2);
        }
        memcpy(mine, src, n * sizeof *mine);
        memcpy(ref, src, n * sizeof *ref);
        qsort(ref, n, sizeof *ref, cmp_int);
    }
    sort_ints(mine, n);
    CHECK(n == 0 || memcmp(mine, ref, n * sizeof *mine) == 0, msg);
    free(mine);
    free(ref);
}

int main(void)
{
    check_fmt(0, 10, "0");
    check_fmt(0, 2, "0");
    check_fmt(7, 8, "7");
    check_fmt(255, 16, "ff");
    check_fmt(255, 2, "11111111");
    check_fmt(12345, 10, "12345");
    check_fmt(4095, 16, "fff");
    check_fmt(1234567890UL, 16, "499602d2");
    check_fmt(100, 7, "202");
    check_fmt(31, 16, "1f");

    static const int mixed[] = {5, -3, 12, 0, -3, 99, 7};
    check_sort(mixed, 7, "sort of mixed values with a duplicate");

    static const int already[] = {-9, -2, 0, 4, 8};
    check_sort(already, 5, "sort of an already sorted array");

    static const int backwards[] = {50, 40, 30, 20, 10};
    check_sort(backwards, 5, "sort of a reverse-sorted array");

    static const int same[] = {6, 6, 6, 6};
    check_sort(same, 4, "sort of all-equal values");

    static const int one[] = {42};
    check_sort(one, 1, "sort of a single element");

    check_sort(NULL, 0, "sort_ints(NULL, 0) does not crash");

    /* 300 pseudo-random values, fixed generator: deterministic. */
    int big[300];
    unsigned long seed = 12345;
    for (size_t i = 0; i < 300; i++) {
        seed = seed * 6364136223846793005UL + 1442695040888963407UL;
        big[i] = (int)((seed >> 33) % 1000) - 500;
    }
    check_sort(big, 300, "sort of 300 pseudo-random values");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
