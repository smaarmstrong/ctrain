/* Test harness — provides main() and drives the learner's quick_sort.
 * Also replaces the library qsort so delegating to it fails the task. */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void quick_sort(void *base, size_t nmemb, size_t size,
                int (*cmp)(const void *, const void *));

/* Link-time stub: any call to the library qsort lands here. */
void qsort(void *base, size_t nmemb, size_t size,
           int (*compar)(const void *, const void *))
{
    (void)base; (void)nmemb; (void)size; (void)compar;
    printf("  FAIL  quick_sort must not call the library qsort\n");
    exit(1);
}

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

static unsigned long rng_state = 0x853c49e6748fea9bUL;
static unsigned rnd(void)
{
    rng_state = rng_state * 6364136223846793005UL + 1442695040888963407UL;
    return (unsigned)(rng_state >> 33);
}

static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

static int cmp_int_desc(const void *a, const void *b)
{
    return cmp_int(b, a);
}

static int cmp_str(const void *a, const void *b)
{
    return strcmp(*(const char *const *)a, *(const char *const *)b);
}

typedef struct {
    int key;
    int shadow; /* always key * 7 + 1: must travel with key */
} Pair;

static int cmp_pair(const void *a, const void *b)
{
    const Pair *x = a, *y = b;
    return (x->key > y->key) - (x->key < y->key);
}

/* keys are all in [0, 65536): histogram before vs after proves a
 * permutation, an adjacent scan proves the order */
enum { KEYSPACE = 65536 };

static int sorted_and_same_multiset(const int *a, const unsigned *before,
                                    size_t n)
{
    unsigned *after = calloc(KEYSPACE, sizeof *after);
    if (!after)
        return 0;
    int ok = 1;
    for (size_t i = 0; i < n; i++) {
        if (a[i] < 0 || a[i] >= KEYSPACE) { ok = 0; break; }
        after[a[i]]++;
        if (i > 0 && a[i - 1] > a[i]) { ok = 0; break; }
    }
    if (ok && memcmp(before, after, KEYSPACE * sizeof *after) != 0)
        ok = 0;
    free(after);
    return ok;
}

static void big_case(const char *msg, void (*fill)(int *, size_t))
{
    enum { N = 100000 };
    int *a = malloc(N * sizeof *a);
    unsigned *before = calloc(KEYSPACE, sizeof *before);
    if (!a || !before) {
        fprintf(stderr, "harness: out of memory\n");
        exit(2);
    }
    fill(a, N);
    for (size_t i = 0; i < N; i++)
        before[a[i]]++;
    quick_sort(a, N, sizeof a[0], cmp_int);
    CHECK(sorted_and_same_multiset(a, before, N), msg);
    free(a);
    free(before);
}

static void fill_sorted(int *a, size_t n)
{
    for (size_t i = 0; i < n; i++)
        a[i] = (int)(i / 2);
}

static void fill_reverse(int *a, size_t n)
{
    for (size_t i = 0; i < n; i++)
        a[i] = (int)((n - 1 - i) / 2);
}

static void fill_random(int *a, size_t n)
{
    for (size_t i = 0; i < n; i++)
        a[i] = (int)(rnd() % KEYSPACE);
}

int main(void)
{
    /* degenerate sizes */
    quick_sort(NULL, 0, sizeof(int), cmp_int);
    CHECK(1, "nmemb == 0 with a NULL base does not crash");
    int single[1] = {41};
    quick_sort(single, 1, sizeof single[0], cmp_int);
    CHECK(single[0] == 41, "one element is left alone");

    /* small int array with duplicates and negatives, exact result */
    {
        int a[7] = {5, -2, 9, 0, 5, 3, -2};
        const int want[7] = {-2, -2, 0, 3, 5, 5, 9};
        quick_sort(a, 7, sizeof a[0], cmp_int);
        CHECK(memcmp(a, want, sizeof want) == 0,
              "seven ints ascending (duplicates, negatives)");
    }

    /* the comparator defines the order: descending */
    {
        int a[6] = {4, 8, 1, 8, 0, 2};
        const int want[6] = {8, 8, 4, 2, 1, 0};
        quick_sort(a, 6, sizeof a[0], cmp_int_desc);
        CHECK(memcmp(a, want, sizeof want) == 0,
              "descending comparator reverses the order");
    }

    /* string pointers via strcmp */
    {
        const char *a[5] = {"pear", "apple", "fig", "banana", "apple"};
        quick_sort(a, 5, sizeof a[0], cmp_str);
        CHECK(strcmp(a[0], "apple") == 0 && strcmp(a[1], "apple") == 0 &&
              strcmp(a[2], "banana") == 0 && strcmp(a[3], "fig") == 0 &&
              strcmp(a[4], "pear") == 0,
              "char * elements sort by strcmp");
    }

    /* structs: the non-key field must travel with its key */
    {
        enum { N = 257 };
        Pair *a = malloc(N * sizeof *a);
        if (!a)
            return 2;
        for (int i = 0; i < N; i++) {
            a[i].key = (int)(rnd() % 40);
            a[i].shadow = a[i].key * 7 + 1;
        }
        quick_sort(a, N, sizeof a[0], cmp_pair);
        int ok = 1;
        for (int i = 0; i < N; i++) {
            if (i > 0 && a[i - 1].key > a[i].key)
                ok = 0;
            if (a[i].shadow != a[i].key * 7 + 1)
                ok = 0;
        }
        CHECK(ok, "struct elements move whole: shadow field stays with key");
        free(a);
    }

    /* all elements equal: must terminate and stay intact */
    {
        enum { N = 512 };
        int *a = malloc(N * sizeof *a);
        if (!a)
            return 2;
        for (int i = 0; i < N; i++)
            a[i] = 77;
        quick_sort(a, N, sizeof a[0], cmp_int);
        int ok = 1;
        for (int i = 0; i < N; i++)
            if (a[i] != 77)
                ok = 0;
        CHECK(ok, "512 all-equal elements survive unchanged");
        free(a);
    }

    /* large arrays: pivot choice must dodge the quadratic cliff */
    big_case("100000 already-sorted ints within the time limit", fill_sorted);
    big_case("100000 reverse-sorted ints within the time limit", fill_reverse);
    big_case("100000 pseudo-random ints, exact multiset kept", fill_random);

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
