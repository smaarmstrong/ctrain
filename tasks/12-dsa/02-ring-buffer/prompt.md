# a FIFO queue on a ring buffer

Implement a fixed-capacity queue of `int`s in `main.c`, stored in a
circular (ring) buffer on the heap. Do **not** define `main` — the grader
links your file against a test harness that provides it.

The harness only ever sees the type as an opaque handle, so `struct queue`
must be defined in *your* file. Provide exactly these declarations:

```c
#include <stdbool.h>
#include <stddef.h>

typedef struct queue Queue;

Queue *queue_new(size_t cap);
void   queue_free(Queue *q);
size_t queue_size(const Queue *q);
size_t queue_cap(const Queue *q);
bool   queue_push(Queue *q, int v);
bool   queue_pop(Queue *q, int *out);
```

Required behaviour:

- `queue_new(cap)` allocates a queue that can hold up to `cap` values.
  It returns `NULL` when `cap` is 0 or an allocation fails.
- `queue_free` releases the queue and everything it owns, whether or not
  elements are still inside. `queue_free(NULL)` is a safe no-op.
- `queue_size` returns how many values are currently stored;
  `queue_cap` returns the capacity it was created with.
- `queue_push` appends `v` at the tail and returns `true`. When the
  queue is full it returns `false` and changes nothing.
- `queue_pop` removes the oldest value into `*out` and returns `true`.
  When the queue is empty it returns `false` and must not touch `*out`.

Values must come out in exactly the order they went in (FIFO). Use head
and count (or head and tail) indices that wrap around the array — do not
shift the remaining elements on every pop. The harness pushes and pops
thousands of times through a small buffer, so the wraparound arithmetic
must be right, and it re-runs everything under AddressSanitizer/UBSan:
off-by-one indexing or a queue freed without its buffer fails the task.

Grade with `ctrain check ring-buffer`.
