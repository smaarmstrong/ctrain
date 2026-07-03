#include <stdbool.h>
#include <stddef.h>

#define STACK_CAP 16

typedef struct {
    int    data[STACK_CAP];
    size_t len;
} IntStack;

void stack_init(IntStack *s)
{
    s->len = 0;
}

size_t stack_size(const IntStack *s)
{
    return s->len;
}

bool stack_push(IntStack *s, int v)
{
    if (s->len == STACK_CAP)
        return false;
    s->data[s->len++] = v;
    return true;
}

bool stack_pop(IntStack *s, int *out)
{
    if (s->len == 0)
        return false;
    *out = s->data[--s->len];
    return true;
}

bool stack_peek(const IntStack *s, int *out)
{
    if (s->len == 0)
        return false;
    *out = s->data[s->len - 1];
    return true;
}
