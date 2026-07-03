/* Test harness — provides main() and drives the learner's sorts. */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int key;
    int tag;
} Rec;

void insertion_sort(Rec *a, size_t n);
void merge_sort(Rec *a, size_t n);

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

/* deterministic pseudo-random keys */
static unsigned long rng_state = 0x9e3779b97f4a7c15UL;
static unsigned rnd(void)
{
    rng_state = rng_state * 6364136223846793005UL + 1442695040888963407UL;
    return (unsigned)(rng_state >> 33);
}

/* a[] was filled with tag == input index. After sorting it must be:
 *  - ascending by key,
 *  - stable (equal keys keep ascending tags),
 *  - a permutation of orig[] (every tag exactly once, key unchanged). */
static int sorted_stable_perm(const Rec *a, const Rec *orig, size_t n)
{
    unsigned char *seen = calloc(n ? n : 1, 1);
    if (!seen)
        return 0;
    int ok = 1;
    for (size_t i = 0; i < n && ok; i++) {
        if (i > 0) {
            if (a[i - 1].key > a[i].key)
                ok = 0;
            else if (a[i - 1].key == a[i].key && a[i - 1].tag >= a[i].tag)
                ok = 0; /* equal keys out of input order: unstable */
        }
        if (ok) {
            if (a[i].tag < 0 || (size_t)a[i].tag >= n || seen[a[i].tag])
                ok = 0;
            else if (a[i].key != orig[a[i].tag].key)
                ok = 0; /* tag no longer paired with its key */
            else
                seen[a[i].tag] = 1;
        }
    }
    free(seen);
    return ok;
}

static Rec *make(size_t n)
{
    Rec *a = malloc((n ? n : 1) * sizeof *a);
    if (!a) {
        fprintf(stderr, "harness: out of memory\n");
        exit(2);
    }
    return a;
}

int main(void)
{
    /* empty input, NULL pointer allowed */
    insertion_sort(NULL, 0);
    CHECK(1, "insertion_sort(NULL, 0) does not crash");
    merge_sort(NULL, 0);
    CHECK(1, "merge_sort(NULL, 0) does not crash");

    /* single element */
    Rec one[1] = {{7, 0}};
    insertion_sort(one, 1);
    CHECK(one[0].key == 7 && one[0].tag == 0, "insertion_sort of one element");
    one[0].key = 9;
    merge_sort(one, 1);
    CHECK(one[0].key == 9 && one[0].tag == 0, "merge_sort of one element");

    /* the fixed example from the prompt: keys 3,1,3,1,2 */
    {
        const int keys[5] = {3, 1, 3, 1, 2};
        const int want_key[5] = {1, 1, 2, 3, 3};
        const int want_tag[5] = {1, 3, 4, 0, 2};
        Rec a[5], b[5];
        for (int i = 0; i < 5; i++) {
            a[i].key = keys[i]; a[i].tag = i;
            b[i] = a[i];
        }
        insertion_sort(a, 5);
        int ok = 1;
        for (int i = 0; i < 5; i++)
            if (a[i].key != want_key[i] || a[i].tag != want_tag[i])
                ok = 0;
        CHECK(ok, "insertion_sort: prompt example, exact stable order");
        merge_sort(b, 5);
        ok = 1;
        for (int i = 0; i < 5; i++)
            if (b[i].key != want_key[i] || b[i].tag != want_tag[i])
                ok = 0;
        CHECK(ok, "merge_sort: prompt example, exact stable order");
    }

    /* reverse-sorted, distinct keys */
    {
        enum { N = 200 };
        Rec *a = make(N), *orig = make(N);
        for (int i = 0; i < N; i++) {
            a[i].key = N - i; a[i].tag = i; orig[i] = a[i];
        }
        insertion_sort(a, N);
        CHECK(sorted_stable_perm(a, orig, N),
              "insertion_sort: 200 reverse-sorted distinct keys");
        for (int i = 0; i < N; i++)
            a[i] = orig[i];
        merge_sort(a, N);
        CHECK(sorted_stable_perm(a, orig, N),
              "merge_sort: 200 reverse-sorted distinct keys");
        free(a); free(orig);
    }

    /* all keys equal: stability means the identity permutation */
    {
        enum { N = 64 };
        Rec *a = make(N);
        for (int i = 0; i < N; i++) {
            a[i].key = 5; a[i].tag = i;
        }
        insertion_sort(a, N);
        int ok = 1;
        for (int i = 0; i < N; i++)
            if (a[i].tag != i)
                ok = 0;
        CHECK(ok, "insertion_sort: all-equal keys stay in input order");
        merge_sort(a, N);
        ok = 1;
        for (int i = 0; i < N; i++)
            if (a[i].tag != i)
                ok = 0;
        CHECK(ok, "merge_sort: all-equal keys stay in input order");
        free(a);
    }

    /* duplicate-heavy pseudo-random arrays */
    {
        enum { N = 2000 };
        Rec *a = make(N), *orig = make(N);
        for (int i = 0; i < N; i++) {
            a[i].key = (int)(rnd() % 25); a[i].tag = i; orig[i] = a[i];
        }
        insertion_sort(a, N);
        CHECK(sorted_stable_perm(a, orig, N),
              "insertion_sort: 2000 records, 25 distinct keys, stable");
        for (int i = 0; i < N; i++)
            a[i] = orig[i];
        merge_sort(a, N);
        CHECK(sorted_stable_perm(a, orig, N),
              "merge_sort: 2000 records, 25 distinct keys, stable");
        free(a); free(orig);
    }

    /* large array: only an O(n log n) merge_sort finishes in time */
    {
        enum { N = 200000 };
        Rec *a = make(N), *orig = make(N);
        for (int i = 0; i < N; i++) {
            a[i].key = (int)(rnd() % 1000); a[i].tag = i; orig[i] = a[i];
        }
        merge_sort(a, N);
        CHECK(sorted_stable_perm(a, orig, N),
              "merge_sort: 200000 records sorted stably within the time limit");
        free(a); free(orig);
    }

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
