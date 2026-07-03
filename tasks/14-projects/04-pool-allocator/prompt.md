# pool — your own malloc and free

Implement a storage allocator that hands out pieces of one caller-provided
buffer. **Do not define `main`** — the grader links its own harness, which
declares these three functions:

```c
#include <stddef.h>

void  pool_init(void *mem, size_t size);
void *pool_alloc(size_t n);
void  pool_free(void *p);
```

## pool_init

Takes ownership of the `size` bytes at `mem` and resets all state — a
second `pool_init` call simply starts over on the new buffer, forgetting
every earlier allocation. The harness always passes a buffer aligned to
`_Alignof(max_align_t)` and at least 1024 bytes, and always calls
`pool_init` before anything else.

**All bookkeeping lives inside the pool** (block headers next to the
blocks, K&R-style). Do not call `malloc`/`free`/etc. — after `pool_init`
your only memory is the buffer. Your total overhead per live allocation
(header plus alignment padding) must not exceed **64 bytes**; the
harness sizes its capacity checks with that allowance.

## pool_alloc

- `pool_alloc(0)` returns `NULL`.
- Otherwise return a pointer to at least `n` usable bytes inside the
  pool, aligned to `_Alignof(max_align_t)`, or `NULL` when no free
  region is big enough. Never hand out overlapping regions: writing all
  `n` bytes of one live allocation must not disturb any other.

## pool_free

- `pool_free(NULL)` is a no-op.
- Otherwise `p` is a pointer previously returned by `pool_alloc` and not
  yet freed; its bytes become available again.
- **Adjacent free blocks must merge.** The harness verifies two
  consequences and your allocator must deliver both:
  - freeing a block between two live ones lets an allocation of the
    same size succeed again when the rest of the pool is full;
  - once *every* allocation has been freed — in any order — the pool
    must again satisfy any single request it could satisfy when fresh
    (e.g. one 60000-byte allocation from a 64 KiB pool).

## What the harness probes (64 KiB pool)

- a fresh pool serves one 60000-byte block, and a second one returns `NULL`
- around 61+ blocks of 1000 bytes fit before `pool_alloc` returns `NULL`
- every returned pointer is `max_align_t`-aligned, even for 1- and
  3-byte requests
- distinct allocations hold their byte patterns independently
- free-then-reallocate in mixed orders, as described above

The whole suite also runs under ASan/UBSan: no misaligned header
accesses, no out-of-bounds arithmetic, no leaks.

Your code goes in `main.c` in the work directory. Grade with
`ctrain check pool-allocator`.
