# a string-keyed hash table that resizes

Implement a hash table mapping C strings to `int`s in `main.c`. Do
**not** define `main` — the grader links your file against a test harness
that provides it. The harness sees the map only as an opaque handle, so
`struct map` (and any entry type) live in *your* file. Provide exactly
these declarations:

```c
#include <stdbool.h>
#include <stddef.h>

typedef struct map Map;

Map   *map_new(void);
void   map_free(Map *m);
size_t map_count(const Map *m);
size_t map_buckets(const Map *m);
bool   map_put(Map *m, const char *key, int value);
bool   map_get(const Map *m, const char *key, int *out);
bool   map_remove(Map *m, const char *key);
```

Use separate chaining or open addressing — your choice. Required
behaviour either way:

- `map_new` returns an empty map with **at least 1 and at most 64
  buckets** (`NULL` on allocation failure). `map_free` releases every
  entry, every key copy, the bucket array, and the map itself;
  `map_free(NULL)` is a safe no-op.
- `map_count` is the number of live key/value pairs; `map_buckets` is
  the current size of the bucket array (for open addressing: the slot
  count).
- `map_put` inserts a new pair or overwrites the value of an existing
  key, returning `true` (`false` only on allocation failure). The map
  must store **its own copy** of the key: the caller may overwrite or
  discard the buffer it passed immediately afterwards.
- `map_get` writes the stored value into `*out` and returns `true`, or
  returns `false` (leaving `*out` untouched) when the key is absent.
- `map_remove` deletes the pair and frees its key copy, returning
  `true`; `false` when the key is absent.

**Resize rule (checked):** whenever `map_put` returns, the load factor
`count / buckets` must be at most 0.75 — grow the bucket array and
rehash the existing entries when an insert would exceed that. Combined
with the ≤ 64 starting buckets, inserting a few thousand keys forces at
least one real rehash, and every earlier mapping must survive it.

The harness inserts 5000 distinct keys, verifies the load-factor bound
via `map_buckets`, re-reads every key after the growth, removes half and
re-checks, and mutates caller buffers to prove keys were copied. It then
repeats everything under AddressSanitizer/UBSan with leak detection: a
key copy lost on remove, overwrite, rehash, or free fails the task.

Grade with `ctrain check hash-table`.
