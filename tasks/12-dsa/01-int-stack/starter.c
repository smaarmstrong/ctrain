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
    /* TODO: store v on top unless the stack is full. */
    (void)s;
    (void)v;
    return false;
}

bool stack_pop(IntStack *s, int *out)
{
    /* TODO: remove the top element into *out unless empty. */
    (void)s;
    (void)out;
    return false;
}

bool stack_peek(const IntStack *s, int *out)
{
    /* TODO: copy the top element into *out without removing it. */
    (void)s;
    (void)out;
    return false;
}
