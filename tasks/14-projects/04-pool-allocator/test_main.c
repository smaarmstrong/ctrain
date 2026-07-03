/* Test harness — provides main() and the pool buffer. */
#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void pool_init(void *mem, size_t size);
void *pool_alloc(size_t n);
void pool_free(void *p);

enum { POOL = 65536 };
static alignas(max_align_t) unsigned char buf[POOL];

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

static int aligned_ok(const void *p)
{
    return p && (uintptr_t)p % _Alignof(max_align_t) == 0;
}

static int in_pool(const void *p, size_t n)
{
    uintptr_t lo = (uintptr_t)buf, hi = lo + POOL, q = (uintptr_t)p;
    return p && q >= lo && q + n <= hi;
}

static int disjoint(const void *p, size_t pn, const void *q, size_t qn)
{
    uintptr_t a = (uintptr_t)p, b = (uintptr_t)q;
    return !p || !q || a + pn <= b || b + qn <= a;
}

static int filled_with(const unsigned char *p, size_t n, unsigned char v)
{
    for (size_t i = 0; i < n; i++)
        if (p[i] != v)
            return 0;
    return 1;
}

int main(void)
{
    /* --- basics ---------------------------------------------------------- */
    pool_init(buf, POOL);
    CHECK(pool_alloc(0) == NULL, "pool_alloc(0) is NULL");
    pool_free(NULL);
    CHECK(1, "pool_free(NULL) does not crash");

    unsigned char *a = pool_alloc(100);
    CHECK(aligned_ok(a), "a 100-byte block is returned, max_align_t-aligned");
    CHECK(in_pool(a, 100), "the block lies inside the pool");
    unsigned char *b = pool_alloc(200);
    CHECK(aligned_ok(b) && in_pool(b, 200), "a second block is returned");
    CHECK(disjoint(a, 100, b, 200), "the two blocks do not overlap");
    if (a)
        memset(a, 0xAA, 100);
    if (b)
        memset(b, 0xBB, 200);
    CHECK(a && filled_with(a, 100, 0xAA),
          "writing block B leaves block A's bytes intact");

    unsigned char *tiny1 = pool_alloc(1);
    unsigned char *tiny3 = pool_alloc(3);
    CHECK(aligned_ok(tiny1) && aligned_ok(tiny3),
          "1- and 3-byte requests are still aligned");
    CHECK(disjoint(tiny1, 1, tiny3, 3), "tiny blocks do not overlap");

    /* --- capacity: one huge block, then exhaustion ------------------------ */
    pool_init(buf, POOL);
    unsigned char *big = pool_alloc(60000);
    CHECK(aligned_ok(big) && in_pool(big, 60000),
          "a fresh 64 KiB pool serves one 60000-byte block");
    if (big)
        memset(big, 0x5C, 60000);
    CHECK(pool_alloc(60000) == NULL,
          "a second 60000-byte request returns NULL");
    unsigned char *tail = pool_alloc(4000);
    CHECK(aligned_ok(tail) && disjoint(big, 60000, tail, 4000),
          "the leftover still serves a 4000-byte block");
    CHECK(big && filled_with(big, 60000, 0x5C),
          "the huge block's bytes survived the later allocations");

    /* --- re-init forgets everything --------------------------------------- */
    pool_init(buf, POOL);
    CHECK(pool_alloc(60000) != NULL,
          "pool_init resets state: the huge block fits again");

    /* --- fill with 1000-byte blocks, free all, coalesce fully -------------- */
    pool_init(buf, POOL);
    unsigned char *blocks[200];
    int count = 0;
    while (count < 200) {
        unsigned char *p = pool_alloc(1000);
        if (!p)
            break;
        memset(p, (unsigned char)count, 1000);
        blocks[count++] = p;
    }
    CHECK(count >= 61, "at least 61 blocks of 1000 bytes fit (low overhead)");
    CHECK(count < 200, "the pool does eventually run out");
    int patterns_ok = 1;
    for (int i = 0; i < count; i++)
        if (!filled_with(blocks[i], 1000, (unsigned char)i))
            patterns_ok = 0;
    CHECK(patterns_ok, "every block still holds its own byte pattern");
    for (int i = 0; i < count; i += 2) /* evens first ... */
        pool_free(blocks[i]);
    for (int i = 1; i < count; i += 2) /* ... then odds */
        pool_free(blocks[i]);
    CHECK(pool_alloc(60000) != NULL,
          "after freeing everything, free blocks merged back into one");

    /* --- freeing a middle block makes exactly that room reusable ----------- */
    pool_init(buf, POOL);
    unsigned char *x = pool_alloc(20000);
    unsigned char *y = pool_alloc(20000);
    unsigned char *z = pool_alloc(20000);
    CHECK(x && y && z, "three 20000-byte blocks fit");
    if (x)
        memset(x, 0x11, 20000);
    if (z)
        memset(z, 0x33, 20000);
    pool_free(y);
    unsigned char *w = pool_alloc(20000);
    CHECK(w != NULL, "the freed middle block serves an equal-size request");
    CHECK(disjoint(w, 20000, x, 20000) && disjoint(w, 20000, z, 20000),
          "the replacement does not overlap its live neighbours");
    if (w)
        memset(w, 0x22, 20000);
    CHECK(x && filled_with(x, 20000, 0x11) && z && filled_with(z, 20000, 0x33),
          "the neighbours' bytes are untouched");
    pool_free(x);
    pool_free(w);
    pool_free(z);
    CHECK(pool_alloc(60000) != NULL,
          "free in mixed order still coalesces the whole pool");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
