#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct dlist DList;

struct node {
    int          value;
    struct node *prev;
    struct node *next;
};

struct dlist {
    struct node *head;
    struct node *tail;
    size_t       len;
};

DList *dlist_new(void)
{
    /* TODO: allocate an empty list. */
    return NULL;
}

void dlist_free(DList *l)
{
    /* TODO: free every node, then the list. NULL must be a no-op. */
    (void)l;
}

size_t dlist_size(const DList *l)
{
    (void)l;
    return 0; /* TODO */
}

bool dlist_push_front(DList *l, int v)
{
    /* TODO */
    (void)l;
    (void)v;
    return false;
}

bool dlist_push_back(DList *l, int v)
{
    /* TODO */
    (void)l;
    (void)v;
    return false;
}

bool dlist_pop_front(DList *l, int *out)
{
    /* TODO */
    (void)l;
    (void)out;
    return false;
}

bool dlist_pop_back(DList *l, int *out)
{
    /* TODO */
    (void)l;
    (void)out;
    return false;
}

bool dlist_remove_first(DList *l, int v)
{
    /* TODO: unlink and free the first node equal to v.
     * Handle head, tail, middle, and only-element cases. */
    (void)l;
    (void)v;
    return false;
}
