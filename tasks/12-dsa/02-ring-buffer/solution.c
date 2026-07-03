#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct queue Queue;

struct queue {
    int   *data;
    size_t cap;
    size_t len;
    size_t head;
};

Queue *queue_new(size_t cap)
{
    if (cap == 0)
        return NULL;
    Queue *q = malloc(sizeof *q);
    if (!q)
        return NULL;
    q->data = malloc(cap * sizeof *q->data);
    if (!q->data) {
        free(q);
        return NULL;
    }
    q->cap = cap;
    q->len = 0;
    q->head = 0;
    return q;
}

void queue_free(Queue *q)
{
    if (!q)
        return;
    free(q->data);
    free(q);
}

size_t queue_size(const Queue *q)
{
    return q->len;
}

size_t queue_cap(const Queue *q)
{
    return q->cap;
}

bool queue_push(Queue *q, int v)
{
    if (q->len == q->cap)
        return false;
    q->data[(q->head + q->len) % q->cap] = v;
    q->len++;
    return true;
}

bool queue_pop(Queue *q, int *out)
{
    if (q->len == 0)
        return false;
    *out = q->data[q->head];
    q->head = (q->head + 1) % q->cap;
    q->len--;
    return true;
}
