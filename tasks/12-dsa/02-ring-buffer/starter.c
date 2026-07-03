#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct queue Queue;

struct queue {
    int   *data;   /* backing array of cap ints */
    size_t cap;
    size_t len;
    size_t head;   /* index of the oldest element */
};

Queue *queue_new(size_t cap)
{
    /* TODO: allocate the struct and its backing array. */
    (void)cap;
    return NULL;
}

void queue_free(Queue *q)
{
    /* TODO: release the array, then the struct. */
    (void)q;
}

size_t queue_size(const Queue *q)
{
    (void)q;
    return 0; /* TODO */
}

size_t queue_cap(const Queue *q)
{
    (void)q;
    return 0; /* TODO */
}

bool queue_push(Queue *q, int v)
{
    /* TODO: write at (head + len) % cap unless full. */
    (void)q;
    (void)v;
    return false;
}

bool queue_pop(Queue *q, int *out)
{
    /* TODO: read at head, advance head with wraparound, unless empty. */
    (void)q;
    (void)out;
    return false;
}
