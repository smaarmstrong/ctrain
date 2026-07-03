# a doubly linked list that owns its nodes

Implement a doubly linked list of `int`s in `main.c`. Do **not** define
`main` — the grader links your file against a test harness that provides
it. The harness sees the list only as an opaque handle, so `struct dlist`
(and your node type) live in *your* file. Provide exactly these
declarations:

```c
#include <stdbool.h>
#include <stddef.h>

typedef struct dlist DList;

DList *dlist_new(void);
void   dlist_free(DList *l);
size_t dlist_size(const DList *l);
bool   dlist_push_front(DList *l, int v);
bool   dlist_push_back(DList *l, int v);
bool   dlist_pop_front(DList *l, int *out);
bool   dlist_pop_back(DList *l, int *out);
bool   dlist_remove_first(DList *l, int v);
```

Required behaviour:

- `dlist_new` returns an empty list, or `NULL` if allocation fails.
- `dlist_free` releases every remaining node and the list itself, in any
  state. `dlist_free(NULL)` is a safe no-op.
- `dlist_size` returns the current element count.
- `dlist_push_front` / `dlist_push_back` allocate a node holding `v` at
  the head / tail and return `true` (`false` only on allocation failure).
- `dlist_pop_front` / `dlist_pop_back` remove the head / tail element
  into `*out`, free its node, and return `true`. On an empty list they
  return `false` and must not touch `*out`.
- `dlist_remove_first(l, v)` unlinks and frees the *first* node (counting
  from the head) whose value equals `v`, returning `true`. If no node
  matches it returns `false` and the list is unchanged.

Removal must work at the head, at the tail, in the middle, and on a
single-element list — those are exactly the cases where prev/next
rewiring goes wrong. The list owns every node it allocates: each node is
freed exactly once, whether by a pop, a removal, or `dlist_free`.

The harness verifies deque behaviour at both ends, all the removal
positions, size bookkeeping, and a 2000-element run, then repeats
everything under AddressSanitizer/UBSan with leak detection: a leaked
node, a double free, or a use of a freed node fails the task.

Grade with `ctrain check doubly-linked-list`.
