# bit census: count, locate, rotate, mirror

Four staples of low-level programming, all built from shifts, masks,
and loops (or the clever branch-free tricks, if you know them — the
grader only checks behaviour).

Implement these in `main.c`. Do **not** define `main` — the grader
links your file against a test harness that provides it.

```c
#include <stdint.h>

/* Number of 1-bits in x: popcount32(0) == 0, popcount32(0xFF) == 8,
 * popcount32(UINT32_MAX) == 32. */
int popcount32(uint32_t x);

/* Number of consecutive 0-bits at the least significant end:
 * trailing_zeros32(1) == 0, trailing_zeros32(8) == 3.
 * By convention trailing_zeros32(0) == 32. */
int trailing_zeros32(uint32_t x);

/* x rotated left by k positions: bits shifted off the top reappear
 * at the bottom. k may be ANY unsigned value — only k % 32 matters,
 * and rotl32(x, 0) == x. Take care: on a 32-bit value, `x << 32` and
 * `x >> 32` are undefined behaviour, and the naive
 * (x << k) | (x >> (32 - k)) hits exactly that when k % 32 == 0. */
uint32_t rotl32(uint32_t x, unsigned k);

/* The 32-bit mirror image: bit 0 swaps with bit 31, bit 1 with
 * bit 30, and so on. reverse_bits32(1) == 0x80000000,
 * reverse_bits32(0xF0000000) == 0x0000000F. */
uint32_t reverse_bits32(uint32_t x);
```

The harness checks values first, then rebuilds with UBSan and runs
everything again — an out-of-range shift count fails the task even
when the produced value happens to be right on this machine.

Grade with `ctrain check bit-census`.
