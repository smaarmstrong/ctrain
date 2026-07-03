#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

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
    if (v->len == v->cap) {
        size_t ncap = v->cap ? v->cap * 2 : 8;
        void **ni = realloc(v->items, ncap * sizeof *ni);
        if (!ni)
            return false;
        v->items = ni;
        v->cap = ncap;
    }
    v->items[v->len++] = item;
    return true;
}

void *vec_get(const Vec *v, size_t i)
{
    if (i >= v->len)
        return NULL;
    return v->items[i];
}

bool vec_set(Vec *v, size_t i, void *item)
{
    if (i >= v->len)
        return false;
    v->items[i] = item;
    return true;
}

bool vec_remove(Vec *v, size_t i, void (*free_item)(void *))
{
    if (i >= v->len)
        return false;
    if (free_item)
        free_item(v->items[i]);
    memmove(v->items + i, v->items + i + 1,
            (v->len - i - 1) * sizeof *v->items);
    v->len--;
    return true;
}

long vec_find(const Vec *v, const void *key,
              int (*cmp)(const void *item, const void *key))
{
    for (size_t i = 0; i < v->len; i++)
        if (cmp(v->items[i], key) == 0)
            return (long)i;
    return -1;
}

void vec_map(Vec *v, void (*fn)(void *item, void *ctx), void *ctx)
{
    for (size_t i = 0; i < v->len; i++)
        fn(v->items[i], ctx);
}

void vec_free(Vec *v, void (*free_item)(void *))
{
    if (!v)
        return;
    if (free_item)
        for (size_t i = 0; i < v->len; i++)
            free_item(v->items[i]);
    free(v->items);
    free(v);
}
