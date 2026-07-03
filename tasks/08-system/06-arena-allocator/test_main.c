/* Test harness — provides main() and calls the learner's arena. */
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct arena;
struct arena *arena_create(size_t capacity);
void *arena_alloc(struct arena *a, size_t n);
size_t arena_used(const struct arena *a);
void arena_reset(struct arena *a);
void arena_destroy(struct arena *a);

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

static int aligned16(const void *p)
{
    return ((uintptr_t)p & 15u) == 0;
}

int main(void)
{
    /* ---- accounting and alignment on a 160-byte arena ------------------- */
    struct arena *a = arena_create(160);
    CHECK(a != NULL, "arena_create(160) returns an arena");
    if (!a)
        return 1;
    CHECK(arena_used(a) == 0, "a fresh arena has used == 0");

    unsigned char *p1 = arena_alloc(a, 1);
    CHECK(p1 != NULL && aligned16(p1), "alloc(1) returns 16-byte-aligned memory");
    CHECK(arena_used(a) == 16, "alloc(1) consumes 16 bytes (rounded up)");

    unsigned char *p2 = arena_alloc(a, 17);
    CHECK(p2 != NULL && aligned16(p2), "alloc(17) returns 16-byte-aligned memory");
    CHECK(arena_used(a) == 48, "alloc(17) consumes 32 more bytes");
    CHECK(p1 != p2, "blocks are distinct");

    if (p1 && p2) {
        p1[0] = 0xAA;
        memset(p2, 0xBB, 17);
        CHECK(p1[0] == 0xAA, "writing one block does not disturb another");
    } else {
        failures++;
    }

    CHECK(arena_alloc(a, 0) == NULL, "alloc(0) returns NULL");
    CHECK(arena_used(a) == 48, "alloc(0) consumes nothing");
    CHECK(arena_alloc(a, 200) == NULL, "an oversized request returns NULL");
    CHECK(arena_used(a) == 48, "a failed request consumes nothing");

    unsigned char *p3 = arena_alloc(a, 112); /* cost 112 == exactly the rest */
    CHECK(p3 != NULL, "a request that exactly fills the arena succeeds");
    if (p3)
        memset(p3, 0xCC, 112);
    CHECK(arena_used(a) == 160, "the arena is now full");
    CHECK(arena_alloc(a, 1) == NULL, "a full arena refuses further requests");

    /* ---- reset: free-all, capacity available again ----------------------- */
    arena_reset(a);
    CHECK(arena_used(a) == 0, "reset drops used back to 0");
    unsigned char *q = arena_alloc(a, 160);
    CHECK(q != NULL && aligned16(q), "after reset the full capacity is available");
    if (q) {
        memset(q, 0x11, 160);
        CHECK(q[0] == 0x11 && q[159] == 0x11, "the whole reset arena is writable");
    } else {
        failures++;
    }
    arena_destroy(a);

    /* ---- many blocks: alignment everywhere, no overlap -------------------- */
    enum { NBLK = 100 };
    struct arena *b = arena_create(NBLK * 16);
    CHECK(b != NULL, "arena_create(1600) returns an arena");
    if (!b)
        return 1;
    unsigned char *blk[NBLK];
    int all_ok = 1;
    for (int i = 0; i < NBLK; i++) {
        blk[i] = arena_alloc(b, 16);
        if (!blk[i] || !aligned16(blk[i])) {
            all_ok = 0;
            blk[i] = NULL;
            break;
        }
        memset(blk[i], i & 0xFF, 16);
    }
    CHECK(all_ok, "100 x alloc(16) all succeed, every block aligned");
    if (all_ok) {
        for (int i = 0; i < NBLK && all_ok; i++)
            for (int j = 0; j < 16; j++)
                if (blk[i][j] != (unsigned char)(i & 0xFF)) {
                    all_ok = 0;
                    break;
                }
        CHECK(all_ok, "every block still holds its own pattern (no overlap)");
    } else {
        CHECK(0, "every block still holds its own pattern (no overlap)");
    }
    CHECK(arena_used(b) == NBLK * 16, "used equals the sum of all costs");
    CHECK(arena_alloc(b, 1) == NULL, "the arena is exhausted after 100 blocks");
    arena_destroy(b);

    /* ---- degenerate cases -------------------------------------------------- */
    struct arena *z = arena_create(0);
    CHECK(z != NULL, "a zero-capacity arena can be created");
    if (z) {
        CHECK(arena_alloc(z, 1) == NULL, "…but every request fails");
        CHECK(arena_used(z) == 0, "…and used stays 0");
        arena_destroy(z);
    } else {
        failures += 2;
    }

    arena_destroy(NULL);
    CHECK(1, "arena_destroy(NULL) is a safe no-op");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
