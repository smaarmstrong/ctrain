#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct entry {
    char *key;          /* heap copy */
    int value;
    struct entry *next; /* next in this bucket's chain */
};

struct map {
    size_t nbuckets;
    struct entry **buckets; /* array of nbuckets chain heads */
};

/* TODO: a hash function over all the key's bytes, e.g. h = h*31 + byte. */

struct map *map_create(size_t nbuckets)
{
    /* TODO: allocate the map and a zeroed bucket array. */
    (void)nbuckets;
    return NULL;
}

int map_put(struct map *m, const char *key, int value)
{
    /* TODO: find in the chain and replace, or prepend a new entry
     * holding a heap COPY of key. */
    (void)m;
    (void)key;
    (void)value;
    return -1;
}

int map_get(const struct map *m, const char *key, int *out)
{
    /* TODO */
    (void)m;
    (void)key;
    (void)out;
    return -1;
}

int map_remove(struct map *m, const char *key)
{
    /* TODO: unlink from anywhere in the chain; free entry and key. */
    (void)m;
    (void)key;
    return -1;
}

void map_free(struct map *m)
{
    /* TODO: every entry, every key copy, the bucket array, the map. */
    (void)m;
}
