/* Test harness — provides main() and drives the learner's vector. */
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    void **items;
    size_t len;
    size_t cap;
} Vec;

Vec   *vec_new(void);
size_t vec_len(const Vec *v);
bool   vec_push(Vec *v, void *item);
void  *vec_get(const Vec *v, size_t i);
bool   vec_set(Vec *v, size_t i, void *item);
bool   vec_remove(Vec *v, size_t i, void (*free_item)(void *));
long   vec_find(const Vec *v, const void *key,
                int (*cmp)(const void *item, const void *key));
void   vec_map(Vec *v, void (*fn)(void *item, void *ctx), void *ctx);
void   vec_free(Vec *v, void (*free_item)(void *));

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

static int *box(int x)
{
    int *p = malloc(sizeof *p);
    if (!p) {
        fprintf(stderr, "harness: out of memory\n");
        exit(2);
    }
    *p = x;
    return p;
}

static char *dupstr(const char *s)
{
    size_t n = strlen(s) + 1;
    char *p = malloc(n);
    if (!p) {
        fprintf(stderr, "harness: out of memory\n");
        exit(2);
    }
    memcpy(p, s, n);
    return p;
}

static int cmp_int(const void *item, const void *key)
{
    return *(const int *)item - *(const int *)key;
}

static int cmp_str(const void *item, const void *key)
{
    return strcmp(item, key);
}

static void sum_ints(void *item, void *ctx)
{
    *(long *)ctx += *(int *)item;
}

static void bump_int(void *item, void *ctx)
{
    (void)ctx;
    (*(int *)item)++;
}

int main(void)
{
    vec_free(NULL, free);
    CHECK(1, "vec_free(NULL, ...) is a safe no-op");

    Vec *v = vec_new();
    CHECK(v != NULL, "vec_new returns a vector");
    if (!v) {
        printf("cannot continue without a vector\n");
        return 1;
    }
    CHECK(vec_len(v) == 0, "a fresh vector is empty");
    CHECK(vec_get(v, 0) == NULL, "get on an empty vector is NULL");

    /* push + get: boxed ints 0..9 */
    bool pushed = true;
    for (int i = 0; i < 10; i++)
        pushed = pushed && vec_push(v, box(i * 11));
    CHECK(pushed, "ten pushes succeed");
    CHECK(vec_len(v) == 10, "len is 10 after ten pushes");
    {
        int ok = 1;
        for (int i = 0; i < 10; i++) {
            int *p = vec_get(v, (size_t)i);
            if (!p || *p != i * 11)
                ok = 0;
        }
        CHECK(ok, "get returns each item in push order");
    }
    CHECK(vec_get(v, 10) == NULL, "get past the end is NULL");
    CHECK(vec_get(v, 9999) == NULL, "get far past the end is NULL");

    /* set: replaces without freeing; caller keeps the old pointer */
    {
        int *old = vec_get(v, 3);
        int *neo = box(-1);
        CHECK(vec_set(v, 3, neo), "set inside the range returns true");
        int *got = vec_get(v, 3);
        CHECK(got == neo && *got == -1, "set stored the new item");
        CHECK(old && *old == 33, "the old item is still alive for the caller");
        free(old);

        int *stray = box(0);
        CHECK(!vec_set(v, 10, stray), "set past the end returns false");
        CHECK(vec_len(v) == 10, "a rejected set changes nothing");
        free(stray);
    }

    /* find with a comparator */
    {
        int want = 77;
        CHECK(vec_find(v, &want, cmp_int) == 7, "find locates a boxed int");
        int missing = 5000;
        CHECK(vec_find(v, &missing, cmp_int) == -1, "find misses cleanly");
        int dup = -1;
        CHECK(vec_find(v, &dup, cmp_int) == 3,
              "find returns the LOWEST matching index");
    }

    /* map: fold a sum through ctx, then mutate in place */
    {
        long sum = 0;
        vec_map(v, sum_ints, &sum);
        /* 0+11+22+(-1)+44+...+99 = sum of i*11 for i!=3, minus 1 */
        long want = 0;
        for (int i = 0; i < 10; i++)
            want += (i == 3) ? -1 : i * 11;
        CHECK(sum == want, "map folds every item through ctx in order");
        vec_map(v, bump_int, NULL);
        int *p0 = vec_get(v, 0), *p9 = vec_get(v, 9);
        CHECK(p0 && *p0 == 1 && p9 && *p9 == 100, "map can mutate items");
    }

    /* remove: middle (freeing), then front with NULL free_item */
    {
        CHECK(vec_remove(v, 3, free), "remove in range returns true");
        CHECK(vec_len(v) == 9, "len drops after remove");
        int *p3 = vec_get(v, 3);
        CHECK(p3 && *p3 == 45, "items after the hole shift left in order");

        int *front = vec_get(v, 0);
        CHECK(vec_remove(v, 0, NULL), "remove with NULL free_item succeeds");
        CHECK(front && *front == 1, "NULL free_item leaves the item alive");
        free(front);
        int *newfront = vec_get(v, 0);
        CHECK(newfront && *newfront == 12, "front removal shifts index 1 to 0");

        CHECK(!vec_remove(v, vec_len(v), free), "remove past the end is false");
        CHECK(vec_len(v) == 8, "a rejected remove changes nothing");
    }

    vec_free(v, free);
    CHECK(1, "vec_free with a destructor (leak-checked)");

    /* strings: find via strcmp, destroy via free */
    {
        Vec *sv = vec_new();
        CHECK(sv != NULL, "second vector for strings");
        if (sv) {
            const char *words[] = {"delta", "alpha", "charlie", "bravo",
                                   "alpha"};
            for (size_t i = 0; i < 5; i++)
                vec_push(sv, dupstr(words[i]));
            CHECK(vec_find(sv, "charlie", cmp_str) == 2,
                  "find works on strings with strcmp");
            CHECK(vec_find(sv, "alpha", cmp_str) == 1,
                  "first matching string wins");
            CHECK(vec_find(sv, "zulu", cmp_str) == -1,
                  "absent string gives -1");
            vec_free(sv, free);
        }
    }

    /* non-owned items: vec_free(v, NULL) must not free them */
    {
        static int stat_a = 1, stat_b = 2;
        int stack_c = 3;
        Vec *nv = vec_new();
        CHECK(nv != NULL, "third vector for borrowed pointers");
        if (nv) {
            vec_push(nv, &stat_a);
            vec_push(nv, &stat_b);
            vec_push(nv, &stack_c);
            CHECK(vec_len(nv) == 3 && vec_get(nv, 2) == &stack_c,
                  "borrowed pointers stored verbatim");
            vec_free(nv, NULL);
            CHECK(stat_a == 1 && stack_c == 3,
                  "vec_free(v, NULL) leaves items untouched");
        }
    }

    /* growth: 50000 pushes must stay fast even under the memory checker */
    {
        enum { N = 50000 };
        Vec *bv = vec_new();
        CHECK(bv != NULL, "big vector allocates");
        if (bv) {
            bool ok = true;
            for (int i = 0; i < N && ok; i++)
                ok = vec_push(bv, box(i));
            CHECK(ok && vec_len(bv) == N, "50000 pushes all succeed");
            int *first = vec_get(bv, 0);
            int *mid = vec_get(bv, N / 2);
            int *last = vec_get(bv, N - 1);
            CHECK(first && *first == 0 && mid && *mid == N / 2 &&
                  last && *last == N - 1,
                  "items survive every reallocation intact");
            vec_free(bv, free);
        }
    }

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
