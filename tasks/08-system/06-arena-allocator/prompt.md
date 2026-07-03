# a bump arena allocator with free-all

Classic UNIX storage allocators carve blocks out of one big region
obtained from the kernel. Build the modern, simple cousin: an **arena**
(bump) allocator — allocation is a pointer increment, and instead of
freeing blocks one by one, the whole arena is reset or destroyed at
once.

Implement it in `main.c`. Do **not** define `main` — the grader links
your file against a test harness that provides it.

```c
#include <stddef.h>

struct arena; /* your definition — the harness treats it as opaque */

/* Make an arena able to hand out `capacity` bytes in total.
 * capacity may be 0 (a valid arena where every request fails).
 * Return NULL only if memory for the arena cannot be obtained. */
struct arena *arena_create(size_t capacity);

/* Hand out n bytes from the arena, or NULL. */
void *arena_alloc(struct arena *a, size_t n);

/* Total bytes consumed so far — see the accounting rules. */
size_t arena_used(const struct arena *a);

/* Free-all: every pointer handed out becomes invalid, used drops to 0,
 * and the full capacity is available again. Must not release the
 * region back to the system — resetting is how arenas stay cheap. */
void arena_reset(struct arena *a);

/* Release everything, arena object included. Must be safe to call
 * with NULL (a no-op). */
void arena_destroy(struct arena *a);
```

## Exact accounting rules (the harness checks these numbers)

- Every returned pointer is aligned to **16 bytes**, and each
  allocation **consumes `n` rounded up to the next multiple of 16**
  ("the cost"). So after `arena_alloc(a, 1)` then `arena_alloc(a, 17)`
  on a fresh arena, `arena_used(a) == 48`.
- `arena_alloc` returns NULL — consuming **nothing** — when `n` is 0,
  or when the cost exceeds `capacity - used`. A request whose cost
  exactly equals the remaining space succeeds.
- `arena_used` on a fresh or freshly reset arena is 0.
- Blocks never overlap: the caller may write all `n` requested bytes
  of every live block, and each block keeps its contents until the
  next reset/destroy.

## Memory discipline (the harness runs everything under ASan/UBSan)

- `arena_destroy` releases every byte the arena ever obtained — a
  leak fails the task.
- Get the region's 16-byte alignment right for every block, not just
  the first one.
- Do not touch memory you did not allocate; UBSan is watching pointer
  arithmetic too.

A hint on shape: one `malloc` for the region (plus one for the
bookkeeping struct, if you like) at create time, an offset that bumps
by the rounded cost per allocation, `reset` sets the offset to 0,
`destroy` frees. Per-allocation `malloc` calls defeat the point —
and the reset/leak checks make that road painful anyway.

Grade with `ctrain check arena-allocator`.
