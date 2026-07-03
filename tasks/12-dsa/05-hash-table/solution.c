#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct map Map;

struct entry {
    char         *key;
    int           value;
    struct entry *next;
};

struct map {
    struct entry **buckets;
    size_t         nbuckets;
    size_t         count;
};

static size_t hash_str(const char *s)
{
    size_t h = 5381;
    while (*s)
        h = h * 33 + (unsigned char)*s++;
    return h;
}

Map *map_new(void)
{
    Map *m = malloc(sizeof *m);
    if (!m)
        return NULL;
    m->nbuckets = 16;
    m->count = 0;
    m->buckets = calloc(m->nbuckets, sizeof *m->buckets);
    if (!m->buckets) {
        free(m);
        return NULL;
    }
    return m;
}

void map_free(Map *m)
{
    if (!m)
        return;
    for (size_t i = 0; i < m->nbuckets; i++) {
        struct entry *e = m->buckets[i];
        while (e) {
            struct entry *next = e->next;
            free(e->key);
            free(e);
            e = next;
        }
    }
    free(m->buckets);
    free(m);
}

size_t map_count(const Map *m)
{
    return m->count;
}

size_t map_buckets(const Map *m)
{
    return m->nbuckets;
}

static struct entry *find_entry(const Map *m, const char *key)
{
    struct entry *e = m->buckets[hash_str(key) % m->nbuckets];
    while (e && strcmp(e->key, key) != 0)
        e = e->next;
    return e;
}

static bool grow(Map *m)
{
    size_t newn = m->nbuckets * 2;
    struct entry **nb = calloc(newn, sizeof *nb);
    if (!nb)
        return false;
    for (size_t i = 0; i < m->nbuckets; i++) {
        struct entry *e = m->buckets[i];
        while (e) {
            struct entry *next = e->next;
            size_t b = hash_str(e->key) % newn;
            e->next = nb[b];
            nb[b] = e;
            e = next;
        }
    }
    free(m->buckets);
    m->buckets = nb;
    m->nbuckets = newn;
    return true;
}

bool map_put(Map *m, const char *key, int value)
{
    struct entry *e = find_entry(m, key);
    if (e) {
        e->value = value;
        return true;
    }
    /* keep count/buckets <= 0.75 after the insert */
    if (4 * (m->count + 1) > 3 * m->nbuckets && !grow(m))
        return false;
    e = malloc(sizeof *e);
    if (!e)
        return false;
    size_t len = strlen(key) + 1;
    e->key = malloc(len);
    if (!e->key) {
        free(e);
        return false;
    }
    memcpy(e->key, key, len);
    e->value = value;
    size_t b = hash_str(key) % m->nbuckets;
    e->next = m->buckets[b];
    m->buckets[b] = e;
    m->count++;
    return true;
}

bool map_get(const Map *m, const char *key, int *out)
{
    const struct entry *e = find_entry(m, key);
    if (!e)
        return false;
    *out = e->value;
    return true;
}

bool map_remove(Map *m, const char *key)
{
    struct entry **link = &m->buckets[hash_str(key) % m->nbuckets];
    while (*link && strcmp((*link)->key, key) != 0)
        link = &(*link)->next;
    struct entry *e = *link;
    if (!e)
        return false;
    *link = e->next;
    free(e->key);
    free(e);
    m->count--;
    return true;
}
