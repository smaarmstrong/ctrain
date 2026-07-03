#include <stddef.h>
#include <stdlib.h>

struct node {
    int value;
    struct node *next;
};

static struct node *node_new(int value, struct node *next)
{
    struct node *n = malloc(sizeof *n);
    if (!n)
        return NULL;
    n->value = value;
    n->next = next;
    return n;
}

struct node *list_from_array(const int *vals, size_t n)
{
    struct node *head = NULL, *tail = NULL;
    for (size_t i = 0; i < n; i++) {
        struct node *nn = node_new(vals[i], NULL);
        if (!nn)
            break; /* grader guarantees this never happens */
        if (tail)
            tail->next = nn;
        else
            head = nn;
        tail = nn;
    }
    return head;
}

struct node *list_insert_sorted(struct node *head, int value)
{
    if (!head || value < head->value)
        return node_new(value, head);
    struct node *cur = head;
    while (cur->next && cur->next->value < value)
        cur = cur->next;
    cur->next = node_new(value, cur->next);
    return head;
}

size_t list_length(const struct node *head)
{
    size_t n = 0;
    for (; head; head = head->next)
        n++;
    return n;
}

void list_free(struct node *head)
{
    while (head) {
        struct node *next = head->next;
        free(head);
        head = next;
    }
}
