#include <stddef.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t len;
    size_t cap;
} intvec;

void vec_init(intvec *v)
{
    v->data = NULL;
    v->len = 0;
    v->cap = 0;
}

int vec_push(intvec *v, int x)
{
    if (v->len == v->cap) {
        size_t newcap = v->cap == 0 ? 8 : v->cap * 2;
        int *nd = realloc(v->data, newcap * sizeof *nd);
        if (!nd)
            return -1;
        v->data = nd;
        v->cap = newcap;
    }
    v->data[v->len++] = x;
    return 0;
}

int vec_pop(intvec *v, int *out)
{
    if (v->len == 0)
        return -1;
    *out = v->data[--v->len];
    return 0;
}

void vec_free(intvec *v)
{
    free(v->data);
    vec_init(v);
}
