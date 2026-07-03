#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct entry {
    char *key;
    int value;
    struct entry *next;
};

struct map {
    size_t nbuckets;
    struct entry **buckets;
};

static size_t hash(const struct map *m, const char *s)
{
    size_t h = 0;
    for (; *s; s++)
        h = h * 31 + (unsigned char)*s;
    return h % m->nbuckets;
}

static char *copy_key(const char *s)
{
    char *p = malloc(strlen(s) + 1);
    if (p)
        strcpy(p, s);
    return p;
}

struct map *map_create(size_t nbuckets)
{
    struct map *m = malloc(sizeof *m);
    if (!m)
        return NULL;
    m->nbuckets = nbuckets;
    m->buckets = calloc(nbuckets, sizeof *m->buckets);
    if (!m->buckets) {
        free(m);
        return NULL;
    }
    return m;
}

static struct entry *find(const struct map *m, const char *key)
{
    for (struct entry *e = m->buckets[hash(m, key)]; e; e = e->next)
        if (strcmp(e->key, key) == 0)
            return e;
    return NULL;
}

int map_put(struct map *m, const char *key, int value)
{
    struct entry *e = find(m, key);
    if (e) {
        e->value = value;
        return 0;
    }
    e = malloc(sizeof *e);
    if (!e)
        return -1;
    e->key = copy_key(key);
    if (!e->key) {
        free(e);
        return -1;
    }
    e->value = value;
    size_t b = hash(m, key);
    e->next = m->buckets[b];
    m->buckets[b] = e;
    return 0;
}

int map_get(const struct map *m, const char *key, int *out)
{
    const struct entry *e = find(m, key);
    if (!e)
        return -1;
    *out = e->value;
    return 0;
}

int map_remove(struct map *m, const char *key)
{
    struct entry **link = &m->buckets[hash(m, key)];
    for (; *link; link = &(*link)->next) {
        if (strcmp((*link)->key, key) == 0) {
            struct entry *e = *link;
            *link = e->next;
            free(e->key);
            free(e);
            return 0;
        }
    }
    return -1;
}

void map_free(struct map *m)
{
    if (!m)
        return;
    for (size_t b = 0; b < m->nbuckets; b++) {
        struct entry *e = m->buckets[b];
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
