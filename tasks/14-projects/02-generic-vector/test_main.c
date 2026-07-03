/* Test harness — provides main() and exercises the vec API. */
#include "vec.h"

#include <stdio.h>
#include <string.h>

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

struct point {
    double x, y;
    int tag;
};

int main(void)
{
    /* --- construction / failure cases ----------------------------------- */
    CHECK(vec_new(0) == NULL, "vec_new(0) returns NULL");

    vec *v = vec_new(sizeof(int));
    CHECK(v != NULL, "vec_new(sizeof(int)) returns a vector");
    if (!v) {
        printf("cannot continue without a vector\n");
        return 1;
    }
    CHECK(vec_len(v) == 0, "a new vector is empty");
    CHECK(vec_get(v, 0) == NULL, "vec_get on an empty vector is NULL");
    CHECK(vec_pop(v, NULL) == -1, "vec_pop on an empty vector is -1");

    /* --- push / get / mutate through the pointer ------------------------- */
    for (int i = 0; i < 5; i++) {
        int val = i * 10;
        CHECK(vec_push(v, &val) == 0, "vec_push succeeds");
    }
    CHECK(vec_len(v) == 5, "length is 5 after five pushes");
    int all_ok = 1;
    for (size_t i = 0; i < 5; i++) {
        int *p = vec_get(v, i);
        if (!p || *p != (int)i * 10)
            all_ok = 0;
    }
    CHECK(all_ok, "vec_get returns each stored value");
    CHECK(vec_get(v, 5) == NULL, "vec_get past the end is NULL");

    int *slot = vec_get(v, 2);
    if (slot)
        *slot = 777;
    slot = vec_get(v, 2);
    CHECK(slot && *slot == 777, "writing through vec_get changes the element");

    /* --- insert ----------------------------------------------------------- */
    int ins = -1;
    CHECK(vec_insert(v, 0, &ins) == 0, "insert at the front succeeds");
    slot = vec_get(v, 0);
    CHECK(slot && *slot == -1, "front insert lands at index 0");
    slot = vec_get(v, 1);
    CHECK(slot && *slot == 0, "old elements shifted up by one");

    ins = -2;
    CHECK(vec_insert(v, 3, &ins) == 0, "insert in the middle succeeds");
    slot = vec_get(v, 3);
    CHECK(slot && *slot == -2, "middle insert lands at its index");

    ins = -3;
    size_t end = vec_len(v);
    CHECK(vec_insert(v, end, &ins) == 0, "insert at i == len appends");
    slot = vec_get(v, end);
    CHECK(slot && *slot == -3, "appended element is last");

    CHECK(vec_insert(v, vec_len(v) + 1, &ins) == -1,
          "insert past len is rejected with -1");
    CHECK(vec_len(v) == 8, "length is 8 after three inserts");

    /* --- remove / pop ------------------------------------------------------ */
    int out = 0;
    CHECK(vec_remove(v, 0, &out) == 0 && out == -1,
          "remove(0) hands back the front element");
    slot = vec_get(v, 0);
    CHECK(slot && *slot == 0, "tail shifted down after remove");
    CHECK(vec_remove(v, vec_len(v), &out) == -1,
          "remove at i == len is rejected with -1");
    CHECK(vec_remove(v, 3, NULL) == 0, "remove with out == NULL works");
    CHECK(vec_len(v) == 6, "length is 6 after two removes");

    CHECK(vec_pop(v, &out) == 0 && out == -3, "pop hands back the last element");
    CHECK(vec_pop(v, NULL) == 0, "pop with out == NULL works");
    CHECK(vec_len(v) == 4, "length is 4 after two pops");
    vec_free(v);

    /* --- genericity: struct elements --------------------------------------- */
    vec *pv = vec_new(sizeof(struct point));
    CHECK(pv != NULL, "a vector of structs can be created");
    if (pv) {
        for (int i = 0; i < 4; i++) {
            struct point p = { i * 1.5, i * -2.5, i };
            CHECK(vec_push(pv, &p) == 0, "pushing a struct succeeds");
        }
        struct point got;
        CHECK(vec_remove(pv, 1, &got) == 0 && got.tag == 1 &&
              got.x == 1.5 && got.y == -2.5,
              "a removed struct comes back intact");
        struct point *pp = vec_get(pv, 2);
        CHECK(pp && pp->tag == 3, "struct elements shift as whole units");
        vec_free(pv);
    }

    /* --- growth under load -------------------------------------------------- */
    vec *big = vec_new(sizeof(int));
    CHECK(big != NULL, "stress vector created");
    if (big) {
        int ok = 1;
        for (int i = 0; i < 50000; i++) {
            if (vec_push(big, &i) != 0) {
                ok = 0;
                break;
            }
        }
        CHECK(ok && vec_len(big) == 50000, "50000 pushes succeed quickly");
        int *first = vec_get(big, 0);
        int *mid = vec_get(big, 25000);
        int *last = vec_get(big, 49999);
        CHECK(first && *first == 0 && mid && *mid == 25000 &&
              last && *last == 49999,
              "elements survive reallocation");
        ok = 1;
        for (int i = 49999; i >= 0; i--) {
            int popped;
            if (vec_pop(big, &popped) != 0 || popped != i) {
                ok = 0;
                break;
            }
        }
        CHECK(ok && vec_len(big) == 0, "popping everything returns LIFO values");
        vec_free(big);
    }

    vec_free(NULL); /* must be a no-op */
    CHECK(1, "vec_free(NULL) does not crash");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
