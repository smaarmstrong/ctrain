/* Test harness — provides main() and drives the learner's list. */
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct dlist DList;

DList *dlist_new(void);
void   dlist_free(DList *l);
size_t dlist_size(const DList *l);
bool   dlist_push_front(DList *l, int v);
bool   dlist_push_back(DList *l, int v);
bool   dlist_pop_front(DList *l, int *out);
bool   dlist_pop_back(DList *l, int *out);
bool   dlist_remove_first(DList *l, int v);

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

/* Build a fresh list from an array via push_back. Returns NULL on failure. */
static DList *build(const int *vals, size_t n)
{
    DList *l = dlist_new();
    if (!l)
        return NULL;
    for (size_t i = 0; i < n; i++)
        if (!dlist_push_back(l, vals[i])) {
            dlist_free(l);
            return NULL;
        }
    return l;
}

/* Drain l front-to-back and compare with vals[0..n). Frees the list. */
static bool drain_equals(DList *l, const int *vals, size_t n)
{
    bool ok = (l != NULL) && dlist_size(l) == n;
    int got;
    for (size_t i = 0; ok && i < n; i++)
        ok = dlist_pop_front(l, &got) && got == vals[i];
    if (ok)
        ok = !dlist_pop_front(l, &got); /* now empty */
    dlist_free(l);
    return ok;
}

int main(void)
{
    dlist_free(NULL);
    CHECK(1, "dlist_free(NULL) does not crash");

    DList *l = dlist_new();
    CHECK(l != NULL, "dlist_new returns a list");
    if (!l) {
        printf("cannot continue without a list\n");
        return 1;
    }
    CHECK(dlist_size(l) == 0, "fresh list is empty");

    int out = 42;
    CHECK(!dlist_pop_front(l, &out), "pop_front on empty returns false");
    CHECK(!dlist_pop_back(l, &out), "pop_back on empty returns false");
    CHECK(out == 42, "failed pops leave *out untouched");
    CHECK(!dlist_remove_first(l, 5), "remove on empty returns false");

    /* deque behaviour at both ends */
    CHECK(dlist_push_back(l, 1) && dlist_push_back(l, 2) &&
          dlist_push_back(l, 3) && dlist_push_front(l, 0),
          "pushes at both ends succeed");
    CHECK(dlist_size(l) == 4, "size is 4 after four pushes");
    CHECK(dlist_pop_front(l, &out) && out == 0, "pop_front sees the front value");
    CHECK(dlist_pop_back(l, &out) && out == 3, "pop_back sees the back value");
    CHECK(dlist_pop_front(l, &out) && out == 1, "pop_front again gives 1");
    CHECK(dlist_pop_back(l, &out) && out == 2, "pop_back drains the last element");
    CHECK(dlist_size(l) == 0, "list is empty after draining");
    dlist_free(l);

    /* remove_first: first match only */
    {
        const int in[] = {5, 7, 5, 9};
        const int want[] = {7, 5, 9};
        l = build(in, 4);
        CHECK(l && dlist_remove_first(l, 5), "remove_first finds a duplicated value");
        CHECK(drain_equals(l, want, 3),
              "only the FIRST matching node is removed");
    }

    /* remove at the head */
    {
        const int in[] = {1, 2, 3};
        const int want[] = {2, 3};
        l = build(in, 3);
        CHECK(l && dlist_remove_first(l, 1), "remove at the head succeeds");
        CHECK(drain_equals(l, want, 2), "head removal keeps the rest intact");
    }

    /* remove at the tail */
    {
        const int in[] = {1, 2, 3};
        const int want[] = {1, 2};
        l = build(in, 3);
        CHECK(l && dlist_remove_first(l, 3), "remove at the tail succeeds");
        CHECK(drain_equals(l, want, 2), "tail removal keeps the rest intact");
    }

    /* remove in the middle */
    {
        const int in[] = {1, 2, 3};
        const int want[] = {1, 3};
        l = build(in, 3);
        CHECK(l && dlist_remove_first(l, 2), "remove in the middle succeeds");
        CHECK(drain_equals(l, want, 2), "middle removal rewires prev/next correctly");
    }

    /* remove the only element */
    {
        const int in[] = {8};
        l = build(in, 1);
        CHECK(l && dlist_remove_first(l, 8), "removing the only element succeeds");
        CHECK(l && dlist_size(l) == 0, "list is empty afterwards");
        CHECK(l && !dlist_remove_first(l, 8), "a second remove returns false");
        /* the emptied list must still be usable */
        CHECK(l && dlist_push_back(l, 4) && dlist_pop_back(l, &out) && out == 4,
              "list still works after removing its only element");
        dlist_free(l);
    }

    /* remove a value that is not there */
    {
        const int in[] = {1, 2, 3};
        l = build(in, 3);
        CHECK(l && !dlist_remove_first(l, 9), "removing an absent value returns false");
        CHECK(drain_equals(l, in, 3), "a failed remove leaves the list unchanged");
    }

    /* larger runs */
    l = dlist_new();
    bool ok = l != NULL;
    for (int i = 0; ok && i < 2000; i++)
        ok = dlist_push_back(l, i);
    CHECK(ok && dlist_size(l) == 2000, "2000 push_backs succeed");
    for (int i = 0; ok && i < 2000; i++)
        ok = dlist_pop_front(l, &out) && out == i;
    CHECK(ok, "2000 elements come out front-to-back in order");
    for (int i = 0; ok && i < 2000; i++)
        ok = dlist_push_front(l, i);
    for (int i = 0; ok && i < 2000; i++)
        ok = dlist_pop_back(l, &out) && out == i;
    CHECK(ok, "push_front then pop_back preserves order too");
    dlist_free(l);

    /* ownership: free a list that still has nodes (leak check catches misses) */
    l = dlist_new();
    ok = l != NULL;
    for (int i = 0; ok && i < 100; i++)
        ok = dlist_push_back(l, i);
    if (ok) {
        dlist_remove_first(l, 50);
        dlist_pop_front(l, &out);
        dlist_pop_back(l, &out);
    }
    CHECK(ok && dlist_size(l) == 97, "mixed removals leave 97 nodes");
    dlist_free(l);
    CHECK(1, "freeing a non-empty list is allowed");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
