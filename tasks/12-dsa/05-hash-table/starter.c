#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct map Map;

/* Separate chaining sketched out — switch to open addressing if you
 * prefer, as long as the declared API behaves as specified. */
struct entry {
    char         *key;   /* the map's own copy */
    int           value;
    struct entry *next;
};

struct map {
    struct entry **buckets;
    size_t         nbuckets;
    size_t         count;
};

/* A classic string hash (djb2). Feel free to use or replace it. */
static size_t hash_str(const char *s)
{
    size_t h = 5381;
    while (*s)
        h = h * 33 + (unsigned char)*s++;
    return h;
}

Map *map_new(void)
{
    /* TODO: allocate the map and a small bucket array (<= 64 buckets). */
    return NULL;
}

void map_free(Map *m)
{
    /* TODO: free every entry and its key copy, the bucket array, the map. */
    (void)m;
}

size_t map_count(const Map *m)
{
    (void)m;
    return 0; /* TODO */
}

size_t map_buckets(const Map *m)
{
    (void)m;
    return 0; /* TODO */
}

bool map_put(Map *m, const char *key, int value)
{
    /* TODO: overwrite if present; otherwise copy the key and insert.
     * Grow + rehash first when count/buckets would exceed 0.75. */
    (void)m;
    (void)key;
    (void)value;
    (void)hash_str;
    return false;
}

bool map_get(const Map *m, const char *key, int *out)
{
    /* TODO */
    (void)m;
    (void)key;
    (void)out;
    return false;
}

bool map_remove(Map *m, const char *key)
{
    /* TODO: unlink, free the key copy and the entry. */
    (void)m;
    (void)key;
    return false;
}
