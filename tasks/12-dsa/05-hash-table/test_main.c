/* Test harness — provides main() and drives the learner's hash map. */
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct map Map;

Map   *map_new(void);
void   map_free(Map *m);
size_t map_count(const Map *m);
size_t map_buckets(const Map *m);
bool   map_put(Map *m, const char *key, int value);
bool   map_get(const Map *m, const char *key, int *out);
bool   map_remove(Map *m, const char *key);

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

/* load factor bound: count/buckets <= 0.75  <=>  4*count <= 3*buckets */
static bool load_ok(const Map *m)
{
    return map_buckets(m) > 0 &&
           4 * map_count(m) <= 3 * map_buckets(m);
}

#define NKEYS 5000

int main(void)
{
    map_free(NULL);
    CHECK(1, "map_free(NULL) does not crash");

    Map *m = map_new();
    CHECK(m != NULL, "map_new returns a map");
    if (!m) {
        printf("cannot continue without a map\n");
        return 1;
    }
    CHECK(map_count(m) == 0, "fresh map holds nothing");
    CHECK(map_buckets(m) >= 1 && map_buckets(m) <= 64,
          "fresh map has between 1 and 64 buckets");

    int out = 42;
    CHECK(!map_get(m, "missing", &out), "get of an absent key returns false");
    CHECK(out == 42, "a failed get leaves *out untouched");
    CHECK(!map_remove(m, "missing"), "remove of an absent key returns false");

    /* basic put/get/update/remove */
    CHECK(map_put(m, "alpha", 1) && map_put(m, "beta", 2) && map_put(m, "gamma", 3),
          "three puts succeed");
    CHECK(map_count(m) == 3, "count is 3 after three distinct puts");
    CHECK(map_get(m, "alpha", &out) && out == 1, "alpha maps to 1");
    CHECK(map_get(m, "gamma", &out) && out == 3, "gamma maps to 3");

    CHECK(map_put(m, "alpha", 10), "putting an existing key succeeds");
    CHECK(map_count(m) == 3, "an overwrite does not change the count");
    CHECK(map_get(m, "alpha", &out) && out == 10, "the value was overwritten");

    CHECK(map_remove(m, "beta"), "remove of a present key returns true");
    CHECK(map_count(m) == 2, "count drops after a remove");
    CHECK(!map_get(m, "beta", &out), "a removed key is gone");
    CHECK(map_get(m, "alpha", &out) && out == 10, "other keys survive a remove");

    /* the map must copy keys, not keep the caller's pointer */
    char buf[16];
    strcpy(buf, "temp-key");
    CHECK(map_put(m, buf, 7), "put from a caller-owned buffer");
    strcpy(buf, "junk!!!!");
    CHECK(map_get(m, "temp-key", &out) && out == 7,
          "the key survives the caller trashing the buffer (key was copied)");
    CHECK(!map_get(m, "junk!!!!", &out), "the trashed buffer content is not a key");
    CHECK(map_remove(m, "temp-key"), "the copied key can be removed");
    map_free(m);

    /* growth phase */
    m = map_new();
    CHECK(m != NULL, "map_new for the growth phase");
    if (!m) {
        printf("cannot continue without a map\n");
        return 1;
    }
    char key[32];
    bool ok = true, load_always_ok = true;
    for (int i = 0; ok && i < NKEYS; i++) {
        snprintf(key, sizeof key, "k%05d", i);
        ok = map_put(m, key, i * 3);
        if ((i & 63) == 0)
            load_always_ok = load_always_ok && load_ok(m);
    }
    CHECK(ok, "5000 distinct puts all succeed");
    CHECK(map_count(m) == NKEYS, "count is 5000");
    CHECK(load_always_ok && load_ok(m),
          "load factor stayed <= 0.75, so the table really grew");
    CHECK(map_buckets(m) > 64, "bucket array is larger than its starting size");

    ok = true;
    for (int i = 0; ok && i < NKEYS; i++) {
        snprintf(key, sizeof key, "k%05d", i);
        ok = map_get(m, key, &out) && out == i * 3;
    }
    CHECK(ok, "every mapping survived the rehashes");

    ok = true;
    for (int i = 0; ok && i < 100; i++) {
        snprintf(key, sizeof key, "absent%03d", i);
        ok = !map_get(m, key, &out);
    }
    CHECK(ok, "100 never-inserted keys all miss");

    /* remove every even key, keep the odd ones */
    ok = true;
    for (int i = 0; ok && i < NKEYS; i += 2) {
        snprintf(key, sizeof key, "k%05d", i);
        ok = map_remove(m, key);
    }
    CHECK(ok, "2500 removes succeed");
    CHECK(map_count(m) == NKEYS / 2, "count is halved");
    ok = true;
    for (int i = 0; ok && i < NKEYS; i++) {
        snprintf(key, sizeof key, "k%05d", i);
        if (i % 2 == 0)
            ok = !map_get(m, key, &out);
        else
            ok = map_get(m, key, &out) && out == i * 3;
    }
    CHECK(ok, "exactly the odd keys remain, values intact");

    /* free a well-populated map — leak check catches lost keys/entries */
    map_free(m);
    CHECK(1, "freeing a populated map is allowed");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
