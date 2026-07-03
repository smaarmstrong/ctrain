#include <stddef.h>
#include <stdlib.h>

/* TODO: the arena owns one region of `capacity` bytes plus a bump
 * offset. Costs are n rounded up to a multiple of 16; every returned
 * pointer must be 16-byte aligned. */
struct arena {
    size_t capacity;
    size_t used;
};

struct arena *arena_create(size_t capacity)
{
    /* TODO: allocate the bookkeeping struct and the region. */
    (void)capacity;
    return NULL;
}

void *arena_alloc(struct arena *a, size_t n)
{
    /* TODO: NULL when n == 0 or the rounded cost does not fit;
     * otherwise bump and return an aligned pointer. */
    (void)a;
    (void)n;
    return NULL;
}

size_t arena_used(const struct arena *a)
{
    /* TODO */
    (void)a;
    return 0;
}

void arena_reset(struct arena *a)
{
    /* TODO: drop used to 0, keep the region. */
    (void)a;
}

void arena_destroy(struct arena *a)
{
    /* TODO: free everything; NULL must be a safe no-op. */
    (void)a;
}
