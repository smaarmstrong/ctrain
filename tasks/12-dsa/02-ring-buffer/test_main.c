/* Test harness — provides main() and drives the learner's queue. */
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct queue Queue;

Queue *queue_new(size_t cap);
void   queue_free(Queue *q);
size_t queue_size(const Queue *q);
size_t queue_cap(const Queue *q);
bool   queue_push(Queue *q, int v);
bool   queue_pop(Queue *q, int *out);

static int failures;

#define CHECK(cond, msg)                                        \
    do {                                                        \
        if (cond) {                                             \
            printf("  ok    %s\n", msg);                        \
        } else {                                                \
            printf("  FAIL  %s\n", msg);                        \
            failures++;                                         \
        }                                                       \
    } while (0)

int main(void)
{
    CHECK(queue_new(0) == NULL, "queue_new(0) returns NULL");
    queue_free(NULL);
    CHECK(1, "queue_free(NULL) does not crash");

    Queue *q = queue_new(3);
    CHECK(q != NULL, "queue_new(3) returns a queue");
    if (!q) {
        printf("cannot continue without a queue\n");
        return 1;
    }
    CHECK(queue_cap(q) == 3, "capacity reads back as 3");
    CHECK(queue_size(q) == 0, "fresh queue is empty");

    int out = 42;
    CHECK(!queue_pop(q, &out), "pop on an empty queue returns false");
    CHECK(out == 42, "a failed pop leaves *out untouched");

    CHECK(queue_push(q, 1) && queue_push(q, 2) && queue_push(q, 3),
          "three pushes fill a capacity-3 queue");
    CHECK(queue_size(q) == 3, "size is 3 when full");
    CHECK(!queue_push(q, 4), "push on a full queue returns false");
    CHECK(queue_size(q) == 3, "a rejected push leaves size unchanged");

    CHECK(queue_pop(q, &out) && out == 1, "the oldest value pops first (FIFO)");
    CHECK(queue_push(q, 4), "there is room again after one pop");
    CHECK(queue_pop(q, &out) && out == 2, "second value out is 2");
    CHECK(queue_pop(q, &out) && out == 3, "third value out is 3");
    CHECK(queue_pop(q, &out) && out == 4, "the value pushed after the wrap comes out last");
    CHECK(queue_size(q) == 0, "queue drains back to empty");
    CHECK(!queue_pop(q, &out), "drained queue rejects another pop");
    queue_free(q);

    /* wraparound stress: keep 3 values in flight through a capacity-5 ring */
    q = queue_new(5);
    CHECK(q != NULL, "queue_new(5) returns a queue");
    if (!q) {
        printf("cannot continue without a queue\n");
        return 1;
    }
    bool ok = true;
    for (int i = 0; i < 10000 && ok; i++) {
        ok = queue_push(q, i);
        if (ok && i >= 3) {
            int v = -1;
            ok = queue_pop(q, &v) && v == i - 3;
        }
    }
    CHECK(ok, "10000 interleaved ops keep FIFO order across many wraps");
    CHECK(queue_size(q) == 3, "exactly 3 values remain in flight");
    /* free with elements still inside: everything must still be released */
    queue_free(q);
    CHECK(1, "freeing a non-empty queue is allowed");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
