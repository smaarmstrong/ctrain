#include <stddef.h>
#include <stdlib.h>

enum { ARENA_ALIGN = 16 };

struct arena {
    unsigned char *base;
    size_t capacity;
    size_t used;
};

static size_t round_up(size_t n)
{
    return (n + (ARENA_ALIGN - 1)) & ~(size_t)(ARENA_ALIGN - 1);
}

struct arena *arena_create(size_t capacity)
{
    struct arena *a = malloc(sizeof *a);
    if (!a)
        return NULL;
    a->base = NULL;
    if (capacity > 0) {
        /* malloc returns memory aligned for max_align_t (16 bytes on
         * the platforms we target), so offsets that are multiples of
         * 16 keep every block aligned. */
        a->base = malloc(capacity);
        if (!a->base) {
            free(a);
            return NULL;
        }
    }
    a->capacity = capacity;
    a->used = 0;
    return a;
}

void *arena_alloc(struct arena *a, size_t n)
{
    if (n == 0)
        return NULL;
    size_t cost = round_up(n);
    if (cost < n || cost > a->capacity - a->used) /* overflow or full */
        return NULL;
    void *p = a->base + a->used;
    a->used += cost;
    return p;
}

size_t arena_used(const struct arena *a)
{
    return a->used;
}

void arena_reset(struct arena *a)
{
    a->used = 0;
}

void arena_destroy(struct arena *a)
{
    if (!a)
        return;
    free(a->base);
    free(a);
}
