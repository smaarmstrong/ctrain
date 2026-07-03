#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct {
    void **items;
    size_t len;
    size_t cap;
} Vec;

Vec *vec_new(void)
{
    Vec *v = malloc(sizeof *v);
    if (!v)
        return NULL;
    v->items = NULL;
    v->len = 0;
    v->cap = 0;
    return v;
}

size_t vec_len(const Vec *v)
{
    return v->len;
}

bool vec_push(Vec *v, void *item)
{
    /* TODO: grow items geometrically when len == cap, then append. */
    (void)v;
    (void)item;
    return false;
}

void *vec_get(const Vec *v, size_t i)
{
    /* TODO: bounds-checked read. */
    (void)v;
    (void)i;
    return NULL;
}

bool vec_set(Vec *v, size_t i, void *item)
{
    /* TODO: bounds-checked replace; never free the old item. */
    (void)v;
    (void)i;
    (void)item;
    return false;
}

bool vec_remove(Vec *v, size_t i, void (*free_item)(void *))
{
    /* TODO: optionally free items[i], shift the tail left, shrink len. */
    (void)v;
    (void)i;
    (void)free_item;
    return false;
}

long vec_find(const Vec *v, const void *key,
              int (*cmp)(const void *item, const void *key))
{
    /* TODO: first index where cmp(items[i], key) == 0, else -1. */
    (void)v;
    (void)key;
    (void)cmp;
    return -1;
}

void vec_map(Vec *v, void (*fn)(void *item, void *ctx), void *ctx)
{
    /* TODO: fn(items[i], ctx) for every i in order. */
    (void)v;
    (void)fn;
    (void)ctx;
}

void vec_free(Vec *v, void (*free_item)(void *))
{
    /* TODO: destroy items (only if free_item), the array, then v. */
    (void)free_item;
    free(v);
}
