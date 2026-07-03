# binary search tree

Implement a binary search tree of unique `int` keys in `main.c`. Do
**not** define `main` — the grader links your file against a test harness
that provides it. The harness sees the tree only as an opaque handle, so
`struct bst` (and your node type) live in *your* file. Provide exactly
these declarations:

```c
#include <stdbool.h>
#include <stddef.h>

typedef struct bst BST;

BST   *bst_new(void);
void   bst_free(BST *t);
size_t bst_size(const BST *t);
bool   bst_insert(BST *t, int key);
bool   bst_contains(const BST *t, int key);
bool   bst_remove(BST *t, int key);
size_t bst_to_array(const BST *t, int *out, size_t max);
```

Required behaviour:

- `bst_new` returns an empty tree (`NULL` on allocation failure);
  `bst_free` releases every node and the tree itself, in any state, and
  accepts `NULL` as a no-op.
- `bst_size` returns the number of keys stored.
- `bst_insert` adds `key` and returns `true`. If the key is already
  present it returns `false` and the tree is unchanged (no duplicates).
- `bst_contains` reports whether `key` is stored.
- `bst_remove` deletes `key` and frees its node, returning `true`;
  if the key is absent it returns `false` and the tree is unchanged.
  All three classic cases must work: a leaf, a node with one child, and
  a node with two children (including when the deleted node is the
  root).
- `bst_to_array` writes the keys in ascending (in-order) sequence into
  `out`, but never more than `max` of them, and returns the number
  written — i.e. the smaller of `max` and the tree's size. With
  `max == 0` it writes nothing and returns 0.

The harness runs a scripted set of insertions and deletions covering all
the removal cases, then a fixed pseudo-random workload of hundreds of
mixed inserts/removes checked against a reference model. Everything is
repeated under AddressSanitizer/UBSan with leak detection: a node lost
during deletion, a double free, or freeing an unbalanced subtree twice
fails the task.

Grade with `ctrain check binary-search-tree`.
