#include "vec.h"

#include <stdlib.h>
#include <string.h>

struct vec {
    size_t elem_size;
    size_t len;
    size_t cap;
    unsigned char *data;
};

vec *vec_new(size_t elem_size)
{
    if (elem_size == 0)
        return NULL;
    vec *v = malloc(sizeof *v);
    if (!v)
        return NULL;
    v->elem_size = elem_size;
    v->len = 0;
    v->cap = 0;
    v->data = NULL;
    return v;
}

void vec_free(vec *v)
{
    if (!v)
        return;
    free(v->data);
    free(v);
}

size_t vec_len(const vec *v)
{
    return v->len;
}

static int ensure_room(vec *v)
{
    if (v->len < v->cap)
        return 0;
    size_t ncap = v->cap ? v->cap * 2 : 8;
    unsigned char *nd = realloc(v->data, ncap * v->elem_size);
    if (!nd)
        return -1;
    v->data = nd;
    v->cap = ncap;
    return 0;
}

int vec_push(vec *v, const void *elem)
{
    if (ensure_room(v) != 0)
        return -1;
    memcpy(v->data + v->len * v->elem_size, elem, v->elem_size);
    v->len++;
    return 0;
}

void *vec_get(const vec *v, size_t i)
{
    if (i >= v->len)
        return NULL;
    return v->data + i * v->elem_size;
}

int vec_pop(vec *v, void *out)
{
    if (v->len == 0)
        return -1;
    v->len--;
    if (out)
        memcpy(out, v->data + v->len * v->elem_size, v->elem_size);
    return 0;
}

int vec_insert(vec *v, size_t i, const void *elem)
{
    if (i > v->len)
        return -1;
    if (ensure_room(v) != 0)
        return -1;
    memmove(v->data + (i + 1) * v->elem_size,
            v->data + i * v->elem_size,
            (v->len - i) * v->elem_size);
    memcpy(v->data + i * v->elem_size, elem, v->elem_size);
    v->len++;
    return 0;
}

int vec_remove(vec *v, size_t i, void *out)
{
    if (i >= v->len)
        return -1;
    if (out)
        memcpy(out, v->data + i * v->elem_size, v->elem_size);
    memmove(v->data + i * v->elem_size,
            v->data + (i + 1) * v->elem_size,
            (v->len - i - 1) * v->elem_size);
    v->len--;
    return 0;
}
