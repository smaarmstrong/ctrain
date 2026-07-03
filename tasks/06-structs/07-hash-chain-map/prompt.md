# hash map with separate chaining

Implement a string-to-int hash map in `main.c`. Do **not** define `main`
— the grader links your file against a test harness that provides it.

The harness only forward-declares `struct map;` — **you** design its
contents. Use an array of `nbuckets` bucket heads, each the head of a
singly linked chain of entries; hash the key to pick a bucket, then walk
the chain comparing with `strcmp`. Any hash function works (the harness
never depends on which bucket a key lands in), but it must use more than
just the first character — `hash = hash * 31 + (unsigned char)*s` over
the bytes is a fine choice.

```c
#include <stddef.h>

struct map;   /* your definition */

/* A map with nbuckets chains (nbuckets >= 1; it is never grown).
 * NULL on allocation failure. */
struct map *map_create(size_t nbuckets);

/* Set key -> value, REPLACING the value if key is already present.
 * The map stores its OWN heap copy of key — the caller may reuse or
 * change the buffer immediately after. Return 0 on success, -1 on
 * allocation failure. */
int map_put(struct map *m, const char *key, int value);

/* If key is present, write its value to *out and return 0.
 * Otherwise return -1 and leave *out untouched. */
int map_get(const struct map *m, const char *key, int *out);

/* Delete key's entry (entry and key copy freed). Return 0 if it was
 * present, -1 if not. Deleting from anywhere in a chain — head, middle
 * or tail — must work. */
int map_remove(struct map *m, const char *key);

/* Free every entry, every key copy, the bucket array and the map
 * itself. map_free(NULL) is a no-op. */
void map_free(struct map *m);
```

Notes:

- `m` and `key` are never NULL (except the documented `map_free(NULL)`).
- Keys in the tests are short (< 32 bytes); values are plain `int`s.
- The harness deliberately creates maps with very few buckets so chains
  collide — a map that only works with one entry per bucket will fail.
- `strdup` may not be declared under `-std=c11`; copy keys with
  `malloc` + `strcpy`.
- The whole harness is rerun under AddressSanitizer: leaking an entry,
  a key copy, the bucket array, or touching a freed entry fails the
  task even when every lookup returns the right value.

Grade with `ctrain check hash-chain-map`.
