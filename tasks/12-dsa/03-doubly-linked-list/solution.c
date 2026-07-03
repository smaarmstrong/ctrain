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
    DList *l = malloc(sizeof *l);
    if (!l)
        return NULL;
    l->head = l->tail = NULL;
    l->len = 0;
    return l;
}

void dlist_free(DList *l)
{
    if (!l)
        return;
    struct node *n = l->head;
    while (n) {
        struct node *next = n->next;
        free(n);
        n = next;
    }
    free(l);
}

size_t dlist_size(const DList *l)
{
    return l->len;
}

static struct node *make_node(int v)
{
    struct node *n = malloc(sizeof *n);
    if (!n)
        return NULL;
    n->value = v;
    n->prev = n->next = NULL;
    return n;
}

bool dlist_push_front(DList *l, int v)
{
    struct node *n = make_node(v);
    if (!n)
        return false;
    n->next = l->head;
    if (l->head)
        l->head->prev = n;
    else
        l->tail = n;
    l->head = n;
    l->len++;
    return true;
}

bool dlist_push_back(DList *l, int v)
{
    struct node *n = make_node(v);
    if (!n)
        return false;
    n->prev = l->tail;
    if (l->tail)
        l->tail->next = n;
    else
        l->head = n;
    l->tail = n;
    l->len++;
    return true;
}

static void unlink_node(DList *l, struct node *n)
{
    if (n->prev)
        n->prev->next = n->next;
    else
        l->head = n->next;
    if (n->next)
        n->next->prev = n->prev;
    else
        l->tail = n->prev;
    free(n);
    l->len--;
}

bool dlist_pop_front(DList *l, int *out)
{
    if (!l->head)
        return false;
    *out = l->head->value;
    unlink_node(l, l->head);
    return true;
}

bool dlist_pop_back(DList *l, int *out)
{
    if (!l->tail)
        return false;
    *out = l->tail->value;
    unlink_node(l, l->tail);
    return true;
}

bool dlist_remove_first(DList *l, int v)
{
    for (struct node *n = l->head; n; n = n->next) {
        if (n->value == v) {
            unlink_node(l, n);
            return true;
        }
    }
    return false;
}
