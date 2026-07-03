#include <stddef.h>
#include <stdlib.h>

struct node {
    int value;
    struct node *next;
};

struct node *list_from_array(const int *vals, size_t n)
{
    /* TODO: malloc one node per value; vals[0] becomes the head. */
    (void)vals;
    (void)n;
    return NULL;
}

struct node *list_insert_sorted(struct node *head, int value)
{
    /* TODO: splice a new node in; watch the new-head case. */
    (void)value;
    return head;
}

size_t list_length(const struct node *head)
{
    /* TODO */
    (void)head;
    return 0;
}

void list_free(struct node *head)
{
    /* TODO: save next before freeing each node. */
    (void)head;
}
