#include <stddef.h>
#include <stdlib.h>

/* Keep this struct EXACTLY as given — the harness relies on it. */
typedef struct {
    int *data;
    size_t len;
    size_t cap;
} intvec;

void vec_init(intvec *v)
{
    /* TODO */
    (void)v;
}

int vec_push(intvec *v, int x)
{
    /* TODO: grow with realloc when len == cap (0 -> 8, then double). */
    (void)v;
    (void)x;
    return -1;
}

int vec_pop(intvec *v, int *out)
{
    /* TODO */
    (void)v;
    (void)out;
    return -1;
}

void vec_free(intvec *v)
{
    /* TODO: release the buffer, reset the fields. */
    (void)v;
}
