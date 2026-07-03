/* Test harness — provides main() and calls the learner's gsort. */
#include <stddef.h>
#include <stdio.h>
#include <string.h>

void gsort(void *base, size_t n, size_t size,
           int (*cmp)(const void *, const void *));

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

static int cmp_int_asc(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

static int cmp_int_desc(const void *a, const void *b)
{
    return cmp_int_asc(b, a);
}

static int cmp_double(const void *a, const void *b)
{
    double x = *(const double *)a, y = *(const double *)b;
    return (x > y) - (x < y);
}

static int cmp_str(const void *a, const void *b)
{
    const char *const *pa = a;
    const char *const *pb = b;
    return strcmp(*pa, *pb);
}

struct planet { char name[8]; int moons; };

static int cmp_moons(const void *a, const void *b)
{
    const struct planet *pa = a, *pb = b;
    return (pa->moons > pb->moons) - (pa->moons < pb->moons);
}

static int int_eq(const int *a, const int *b, size_t n)
{
    return memcmp(a, b, n * sizeof *a) == 0;
}

int main(void)
{
    /* ints ascending, with duplicates */
    int a[9] = {5, -1, 3, 3, 0, 12, -7, 3, 5};
    const int a_sorted[9] = {-7, -1, 0, 3, 3, 3, 5, 5, 12};
    gsort(a, 9, sizeof a[0], cmp_int_asc);
    CHECK(int_eq(a, a_sorted, 9), "ints ascending (with duplicates)");

    /* same data, same gsort, opposite comparator */
    int b[9] = {5, -1, 3, 3, 0, 12, -7, 3, 5};
    const int b_sorted[9] = {12, 5, 5, 3, 3, 3, 0, -1, -7};
    gsort(b, 9, sizeof b[0], cmp_int_desc);
    CHECK(int_eq(b, b_sorted, 9), "ints descending via a different comparator");

    /* degenerate sizes */
    int one[1] = {99};
    gsort(one, 1, sizeof one[0], cmp_int_asc);
    CHECK(one[0] == 99, "n == 1 is a no-op");
    gsort(NULL, 0, sizeof(int), cmp_int_asc);
    CHECK(1, "n == 0 with base == NULL does not crash");

    /* already sorted and reverse sorted */
    int up[5] = {1, 2, 3, 4, 5};
    gsort(up, 5, sizeof up[0], cmp_int_asc);
    CHECK(int_eq(up, (const int[]){1, 2, 3, 4, 5}, 5), "already-sorted input survives");
    int down[5] = {5, 4, 3, 2, 1};
    gsort(down, 5, sizeof down[0], cmp_int_asc);
    CHECK(int_eq(down, (const int[]){1, 2, 3, 4, 5}, 5), "reverse-sorted input");

    /* doubles: a different element size */
    double d[4] = {2.5, -0.5, 10.0, 0.25};
    gsort(d, 4, sizeof d[0], cmp_double);
    CHECK(d[0] == -0.5 && d[1] == 0.25 && d[2] == 2.5 && d[3] == 10.0,
          "doubles sort with an 8-byte element");

    /* pointers to strings */
    const char *w[5] = {"pear", "apple", "fig", "apple", "banana"};
    gsort(w, 5, sizeof w[0], cmp_str);
    CHECK(strcmp(w[0], "apple") == 0 && strcmp(w[1], "apple") == 0 &&
          strcmp(w[2], "banana") == 0 && strcmp(w[3], "fig") == 0 &&
          strcmp(w[4], "pear") == 0,
          "array of char * via strcmp comparator");

    /* structs: element size not a power of two, key in the middle */
    struct planet p[4] = {
        {"saturn", 146}, {"earth", 1}, {"mars", 2}, {"venus", 0},
    };
    gsort(p, 4, sizeof p[0], cmp_moons);
    CHECK(strcmp(p[0].name, "venus") == 0 && strcmp(p[1].name, "earth") == 0 &&
          strcmp(p[2].name, "mars") == 0 && strcmp(p[3].name, "saturn") == 0,
          "structs sorted by one field");

    /* a bigger array: 1000 ints, descending input, worst case for O(n^2) */
    static int big[1000];
    for (int i = 0; i < 1000; i++)
        big[i] = 1000 - i;
    gsort(big, 1000, sizeof big[0], cmp_int_asc);
    int ok = 1;
    for (int i = 0; i < 1000; i++)
        if (big[i] != i + 1)
            ok = 0;
    CHECK(ok, "1000 elements, worst-case order");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
