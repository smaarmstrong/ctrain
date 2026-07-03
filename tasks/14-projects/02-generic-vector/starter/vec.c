#include "vec.h"

#include <stdlib.h>
#include <string.h>

/* TODO: choose a layout, e.g.
 *
 *   struct vec {
 *       size_t elem_size;   // bytes per element
 *       size_t len;         // elements in use
 *       size_t cap;         // elements allocated
 *       unsigned char *data;
 *   };
 *
 * - grow by doubling (start at some small cap) so vec_push stays amortised
 * - element i lives at data + i * elem_size; move ranges with memmove
 * - copy elements with memcpy(dst, src, elem_size)
 */

struct vec {
    size_t elem_size;
};

vec *vec_new(size_t elem_size)
{
    (void)elem_size;
    return NULL; /* TODO */
}

void vec_free(vec *v)
{
    (void)v; /* TODO */
}

size_t vec_len(const vec *v)
{
    (void)v;
    return 0; /* TODO */
}

int vec_push(vec *v, const void *elem)
{
    (void)v;
    (void)elem;
    return -1; /* TODO */
}

void *vec_get(const vec *v, size_t i)
{
    (void)v;
    (void)i;
    return NULL; /* TODO */
}

int vec_pop(vec *v, void *out)
{
    (void)v;
    (void)out;
    return -1; /* TODO */
}

int vec_insert(vec *v, size_t i, const void *elem)
{
    (void)v;
    (void)i;
    (void)elem;
    return -1; /* TODO */
}

int vec_remove(vec *v, size_t i, void *out)
{
    (void)v;
    (void)i;
    (void)out;
    return -1; /* TODO */
}
