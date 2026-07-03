/* Test harness — provides main() and calls the learner's functions. */
#include <stdint.h>
#include <stdio.h>

int popcount32(uint32_t x);
int trailing_zeros32(uint32_t x);
uint32_t rotl32(uint32_t x, unsigned k);
uint32_t reverse_bits32(uint32_t x);

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

int main(void)
{
    /* popcount32 */
    CHECK(popcount32(0) == 0, "popcount32(0) == 0");
    CHECK(popcount32(1) == 1, "popcount32(1) == 1");
    CHECK(popcount32(0xFFu) == 8, "popcount32(0xFF) == 8");
    CHECK(popcount32(0x80000000u) == 1, "only the top bit");
    CHECK(popcount32(UINT32_MAX) == 32, "popcount32(UINT32_MAX) == 32");
    CHECK(popcount32(0xAAAAAAAAu) == 16, "alternating bits");
    CHECK(popcount32(0x12345678u) == 13, "popcount32(0x12345678) == 13");

    /* trailing_zeros32 */
    CHECK(trailing_zeros32(1) == 0, "tz(1) == 0");
    CHECK(trailing_zeros32(2) == 1, "tz(2) == 1");
    CHECK(trailing_zeros32(8) == 3, "tz(8) == 3");
    CHECK(trailing_zeros32(0x80000000u) == 31, "tz(top bit) == 31");
    CHECK(trailing_zeros32(0) == 32, "tz(0) == 32 by convention");
    CHECK(trailing_zeros32(0xFFFFFFFFu) == 0, "tz(all ones) == 0");
    CHECK(trailing_zeros32(0x00010000u) == 16, "tz(1<<16) == 16");

    /* rotl32 */
    CHECK(rotl32(0x00000001u, 1) == 0x00000002u, "rotate 1 left by 1");
    CHECK(rotl32(0x80000000u, 1) == 0x00000001u, "top bit wraps to bottom");
    CHECK(rotl32(0x12345678u, 0) == 0x12345678u, "rotate by 0 is identity");
    CHECK(rotl32(0x12345678u, 32) == 0x12345678u, "rotate by 32 is identity");
    CHECK(rotl32(0x12345678u, 64) == 0x12345678u, "rotate by 64 is identity");
    CHECK(rotl32(0x12345678u, 4) == 0x23456781u, "rotate by a nibble");
    CHECK(rotl32(0x12345678u, 36) == 0x23456781u, "k % 32 is what counts");
    CHECK(rotl32(0xABCD1234u, 16) == 0x1234ABCDu, "rotate by 16 swaps halves");

    /* reverse_bits32 */
    CHECK(reverse_bits32(0) == 0, "reverse(0) == 0");
    CHECK(reverse_bits32(1) == 0x80000000u, "reverse(1) == top bit");
    CHECK(reverse_bits32(0x80000000u) == 1, "reverse(top bit) == 1");
    CHECK(reverse_bits32(0xF0000000u) == 0x0000000Fu, "reverse a nibble");
    CHECK(reverse_bits32(0xAAAAAAAAu) == 0x55555555u,
          "reverse alternating bits");
    CHECK(reverse_bits32(0x00000002u) == 0x40000000u, "reverse bit 1");
    uint32_t x = 0x9D2F4B71u;
    CHECK(reverse_bits32(reverse_bits32(x)) == x,
          "reversing twice restores the original");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
