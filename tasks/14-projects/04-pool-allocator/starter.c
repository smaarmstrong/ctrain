#include <stddef.h>

/* pool — a malloc/free-style allocator over one caller-provided buffer.
 *
 * TODO:
 *   1. pool_init: lay one big free block over the buffer; keep a header
 *      (size / free flag / next-by-address) in front of every block
 *   2. pool_alloc: round n up to _Alignof(max_align_t); first fit; split
 *      a big free block so the remainder stays usable; return the bytes
 *      just past the header
 *   3. pool_free: step back to the header, mark it free, and merge every
 *      pair of adjacent free blocks so big requests fit again
 *
 * Keep per-allocation overhead (header + padding) at 64 bytes or less.
 */

void pool_init(void *mem, size_t size)
{
    (void)mem;
    (void)size;
    /* TODO */
}

void *pool_alloc(size_t n)
{
    (void)n;
    return NULL; /* TODO */
}

void pool_free(void *p)
{
    (void)p;
    /* TODO */
}
