/* Test harness — provides main() and drives the learner's BST against a
 * simple sorted-array reference model. */
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct bst BST;

BST   *bst_new(void);
void   bst_free(BST *t);
size_t bst_size(const BST *t);
bool   bst_insert(BST *t, int key);
bool   bst_contains(const BST *t, int key);
bool   bst_remove(BST *t, int key);
size_t bst_to_array(const BST *t, int *out, size_t max);

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

/* ---- reference model: a sorted array of unique ints ---- */
#define MODEL_MAX 2048
static int    model[MODEL_MAX];
static size_t mn;

static bool model_contains(int key)
{
    for (size_t i = 0; i < mn; i++)
        if (model[i] == key)
            return true;
    return false;
}

static bool model_insert(int key)
{
    if (model_contains(key))
        return false;
    size_t i = mn;
    while (i > 0 && model[i - 1] > key) {
        model[i] = model[i - 1];
        i--;
    }
    model[i] = key;
    mn++;
    return true;
}

static bool model_remove(int key)
{
    for (size_t i = 0; i < mn; i++) {
        if (model[i] == key) {
            memmove(&model[i], &model[i + 1], (mn - i - 1) * sizeof model[0]);
            mn--;
            return true;
        }
    }
    return false;
}

/* deterministic pseudo-random sequence */
static unsigned lcg_state = 20260702u;
static unsigned lcg(void)
{
    lcg_state = lcg_state * 1664525u + 1013904223u;
    return lcg_state >> 16;
}

static bool tree_matches_model(const BST *t)
{
    int got[MODEL_MAX];
    if (bst_size(t) != mn)
        return false;
    if (bst_to_array(t, got, MODEL_MAX) != mn)
        return false;
    return memcmp(got, model, mn * sizeof got[0]) == 0;
}

int main(void)
{
    bst_free(NULL);
    CHECK(1, "bst_free(NULL) does not crash");

    BST *t = bst_new();
    CHECK(t != NULL, "bst_new returns a tree");
    if (!t) {
        printf("cannot continue without a tree\n");
        return 1;
    }
    int buf[16];
    CHECK(bst_size(t) == 0, "fresh tree is empty");
    CHECK(!bst_contains(t, 5), "empty tree contains nothing");
    CHECK(!bst_remove(t, 5), "remove on an empty tree returns false");
    CHECK(bst_to_array(t, buf, 16) == 0, "to_array on an empty tree writes 0 keys");

    /* scripted phase */
    const int keys[] = {5, 3, 8, 1, 4, 7, 9};
    bool ok = true;
    for (size_t i = 0; i < 7; i++)
        ok = ok && bst_insert(t, keys[i]);
    CHECK(ok, "seven distinct inserts all return true");
    CHECK(bst_size(t) == 7, "size is 7");
    CHECK(!bst_insert(t, 5), "inserting a duplicate returns false");
    CHECK(bst_size(t) == 7, "a rejected duplicate leaves size unchanged");

    CHECK(bst_contains(t, 1) && bst_contains(t, 9) && bst_contains(t, 4),
          "contains finds stored keys");
    CHECK(!bst_contains(t, 2) && !bst_contains(t, 6),
          "contains rejects absent keys");

    const int sorted7[] = {1, 3, 4, 5, 7, 8, 9};
    CHECK(bst_to_array(t, buf, 16) == 7 &&
              memcmp(buf, sorted7, sizeof sorted7) == 0,
          "in-order traversal yields ascending keys");
    memset(buf, 0, sizeof buf);
    CHECK(bst_to_array(t, buf, 3) == 3 && buf[0] == 1 && buf[1] == 3 && buf[2] == 4,
          "to_array honours max: only the 3 smallest keys are written");

    CHECK(bst_remove(t, 1), "remove a leaf");
    CHECK(bst_remove(t, 3), "remove a node with one child");
    CHECK(bst_remove(t, 5), "remove the root while it has two children");
    CHECK(!bst_remove(t, 5), "removing an already-removed key returns false");
    const int sorted4[] = {4, 7, 8, 9};
    CHECK(bst_size(t) == 4 && bst_to_array(t, buf, 16) == 4 &&
              memcmp(buf, sorted4, sizeof sorted4) == 0,
          "tree is intact and ordered after all three deletion cases");

    while (bst_size(t) > 0) {
        (void)bst_to_array(t, buf, 1);
        if (!bst_remove(t, buf[0]))
            break;
    }
    CHECK(bst_size(t) == 0, "tree can be emptied one key at a time");
    CHECK(bst_insert(t, 42) && bst_contains(t, 42),
          "an emptied tree accepts new keys");
    CHECK(bst_remove(t, 42), "and can drop them again");
    bst_free(t);

    /* pseudo-random workload against the reference model */
    t = bst_new();
    CHECK(t != NULL, "bst_new for the random phase");
    if (!t) {
        printf("cannot continue without a tree\n");
        return 1;
    }
    mn = 0;
    ok = true;
    for (int i = 0; ok && i < 800; i++) {
        int key = (int)(lcg() % 500);
        ok = bst_insert(t, key) == model_insert(key);
    }
    CHECK(ok, "800 inserts (with duplicates) agree with the model");
    CHECK(tree_matches_model(t), "in-order output matches the sorted model");

    ok = true;
    for (int i = 0; ok && i < 600; i++) {
        int key = (int)(lcg() % 500);
        ok = bst_remove(t, key) == model_remove(key);
    }
    CHECK(ok, "600 removes (some absent) agree with the model");
    CHECK(tree_matches_model(t), "in-order output still matches after removals");

    ok = true;
    for (int key = 0; ok && key < 500; key++)
        ok = bst_contains(t, key) == model_contains(key);
    CHECK(ok, "contains agrees with the model for every key 0..499");

    /* free with plenty of nodes still inside — leak check must stay clean */
    CHECK(bst_size(t) > 50, "a healthy number of keys remain for the free test");
    bst_free(t);
    CHECK(1, "freeing a populated tree is allowed");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
