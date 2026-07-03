#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **items;
    size_t count;
    size_t cap;
} sstack;

void sstack_init(sstack *s)
{
    s->items = NULL;
    s->count = 0;
    s->cap = 0;
}

static int grow(sstack *s)
{
    size_t ncap = s->cap ? s->cap * 2 : 4;
    char **p = realloc(s->items, ncap * sizeof *p);
    if (!p)
        return -1;
    s->items = p;
    s->cap = ncap;
    return 0;
}

int sstack_push(sstack *s, const char *str)
{
    if (s->count == s->cap && grow(s) != 0)
        return -1;
    char *copy = malloc(strlen(str) + 1);
    if (!copy)
        return -1;
    strcpy(copy, str);
    s->items[s->count++] = copy;
    return 0;
}

char *sstack_pop(sstack *s)
{
    if (s->count == 0)
        return NULL;
    s->count--;
    /* tidy up before handing the string out */
    free(s->items[s->count]);
    return s->items[s->count];
}

void sstack_destroy(sstack *s)
{
    for (size_t i = 0; i < s->count; i++)
        free(s->items[i]);
    free(s->items);
}
