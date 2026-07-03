/* Test harness — provides main() and calls the learner's functions. */
#include <stddef.h>
#include <stdio.h>
#include <string.h>

struct map;

struct map *map_create(size_t nbuckets);
int map_put(struct map *m, const char *key, int value);
int map_get(const struct map *m, const char *key, int *out);
int map_remove(struct map *m, const char *key);
void map_free(struct map *m);

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

int main(void)
{
    struct map *m = map_create(4); /* few buckets: collisions guaranteed */
    CHECK(m != NULL, "map_create(4) returns a map");
    if (!m) {
        printf("cannot continue without a map\n");
        return 1;
    }

    int out = 12345;
    CHECK(map_get(m, "missing", &out) == -1, "get on an empty map returns -1");
    CHECK(out == 12345, "failed get leaves *out untouched");

    /* 60 keys into 4 buckets: every chain holds many entries */
    char key[32];
    int ok = 1;
    for (int i = 0; i < 60; i++) {
        snprintf(key, sizeof key, "key%02d", i);
        if (map_put(m, key, i * i) != 0)
            ok = 0;
    }
    CHECK(ok, "60 puts all return 0");

    ok = 1;
    for (int i = 0; i < 60; i++) {
        snprintf(key, sizeof key, "key%02d", i);
        if (map_get(m, key, &out) != 0 || out != i * i)
            ok = 0;
    }
    CHECK(ok, "all 60 values read back through collided chains");

    CHECK(map_put(m, "key07", 777) == 0, "overwriting an existing key returns 0");
    CHECK(map_get(m, "key07", &out) == 0 && out == 777,
          "overwrite replaces the value");
    ok = 1;
    for (int i = 0; i < 60; i++) {
        if (i == 7)
            continue;
        snprintf(key, sizeof key, "key%02d", i);
        if (map_get(m, key, &out) != 0 || out != i * i)
            ok = 0;
    }
    CHECK(ok, "overwrite does not disturb the other 59 entries");

    /* the map must copy keys, not alias the caller's buffer */
    char buf[16];
    strcpy(buf, "alpha");
    map_put(m, buf, 1);
    strcpy(buf, "beta!");
    CHECK(map_get(m, "alpha", &out) == 0 && out == 1,
          "map stores its own copy of the key");
    CHECK(map_get(m, "beta!", &out) == -1,
          "mutating the caller's buffer does not invent a key");

    CHECK(map_remove(m, "key30") == 0, "removing a present key returns 0");
    CHECK(map_get(m, "key30", &out) == -1, "removed key is gone");
    CHECK(map_remove(m, "key30") == -1, "removing it again returns -1");
    CHECK(map_remove(m, "never-there") == -1, "removing an absent key returns -1");

    ok = 1;
    for (int i = 0; i < 10; i++) { /* thin out several chains */
        snprintf(key, sizeof key, "key%02d", i);
        if (map_remove(m, key) != 0)
            ok = 0;
        if (map_get(m, key, &out) != -1)
            ok = 0;
    }
    CHECK(ok, "ten more removals from chain heads, middles and tails");
    ok = 1;
    for (int i = 10; i < 60; i++) {
        if (i == 30)
            continue;
        snprintf(key, sizeof key, "key%02d", i);
        if (map_get(m, key, &out) != 0)
            ok = 0;
    }
    CHECK(ok, "surviving entries are all still reachable");
    map_free(m);

    /* one bucket: the whole map is a single chain */
    struct map *one = map_create(1);
    CHECK(one != NULL, "map_create(1) returns a map");
    if (one) {
        map_put(one, "a", 1);
        map_put(one, "b", 2);
        map_put(one, "c", 3);
        CHECK(map_remove(one, "b") == 0, "unlinking inside a single chain works");
        ok = map_get(one, "a", &out) == 0 && out == 1;
        ok = ok && map_get(one, "c", &out) == 0 && out == 3;
        ok = ok && map_get(one, "b", &out) == -1;
        CHECK(ok, "the chain survives the unlink intact");
        map_free(one);
    } else {
        failures += 2;
    }

    map_free(NULL);
    CHECK(1, "map_free(NULL) does not crash");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
