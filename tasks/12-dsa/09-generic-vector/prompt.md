# a generic vector of void pointers

Implement a growable, type-agnostic container in `main.c`. Do **not**
define `main` — the grader links your file against a test harness that
provides it. The vector stores `void *` items it does not understand;
all type-specific behaviour (destruction, matching, visiting) arrives
through function pointers.

Copy this type and these prototypes into your file exactly as written
(the harness uses the same definitions):

```c
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    void **items;
    size_t len;
    size_t cap;
} Vec;

Vec   *vec_new(void);
size_t vec_len(const Vec *v);
bool   vec_push(Vec *v, void *item);
void  *vec_get(const Vec *v, size_t i);
bool   vec_set(Vec *v, size_t i, void *item);
bool   vec_remove(Vec *v, size_t i, void (*free_item)(void *));
long   vec_find(const Vec *v, const void *key,
                int (*cmp)(const void *item, const void *key));
void   vec_map(Vec *v, void (*fn)(void *item, void *ctx), void *ctx);
void   vec_free(Vec *v, void (*free_item)(void *));
```

Required behaviour:

- `vec_new` heap-allocates an empty vector (`len == 0`) and returns
  it, or `NULL` if allocation fails. `items` may start `NULL` with
  `cap == 0`.
- `vec_len` returns the current element count.
- `vec_push` appends `item` and returns `true`; on allocation failure
  it returns `false` leaving the vector usable and unchanged. Grow the
  `items` array **geometrically** (e.g. double it, with some minimum):
  the harness pushes 50,000 items under a memory checker, and a
  grow-by-one vector is far too slow there.
- `vec_get` returns the item at index `i`, or `NULL` if `i >= len`.
- `vec_set` replaces the item at `i` and returns `true`; when
  `i >= len` it returns `false` and changes nothing. It never frees
  the old item — the caller still owns that pointer.
- `vec_remove` removes the item at `i`, shifting later items left one
  slot (order preserved), and returns `true`; when `i >= len` it
  returns `false` and changes nothing. If `free_item` is non-`NULL`
  it is called on the removed item; if `NULL` the item is simply
  dropped (the caller kept a pointer).
- `vec_find` returns the lowest index `i` for which
  `cmp(items[i], key) == 0` (strcmp-style: zero means match), or `-1`
  when nothing matches.
- `vec_map` calls `fn(item, ctx)` on every item, in index order,
  passing the same `ctx` through each call.
- `vec_free` destroys the vector: if `free_item` is non-`NULL` it is
  called on every stored item first; then the `items` array and the
  `Vec` itself are freed. `vec_free(NULL, anything)` is a safe no-op.
  With a `NULL` `free_item` the stored pointers must be left alone —
  the harness stores non-heap pointers in one test, and freeing them
  would abort under AddressSanitizer.

Except for `vec_free`, the harness never passes a `NULL` vector.
Stored items are never `NULL`.

Everything runs leak-checked under AddressSanitizer/UBSan and
valgrind: a lost `items` array, a double free, or an item freed with
the wrong ownership fails the task even if all return values look
right.

Grade with `ctrain check generic-vector`.
