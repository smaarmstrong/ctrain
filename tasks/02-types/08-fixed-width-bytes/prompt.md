# fixed-width types and a byte protocol

`int` and `long` change size between platforms; file formats and wire
protocols do not. This exercise reads and writes *little-endian*
multi-byte values (least significant byte first) through `uint8_t`
buffers, using the exact-width types from `<stdint.h>`.

Implement these in `main.c`. Do **not** define `main` — the grader
links your file against a test harness that provides it.

```c
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* The 16-bit value stored little-endian at p (reads p[0], p[1]).
 * Bytes {0x34, 0x12} decode to 0x1234. */
uint16_t load_le16(const uint8_t *p);

/* The 32-bit value stored little-endian at p (reads p[0]..p[3]).
 * Bytes {0x78, 0x56, 0x34, 0x12} decode to 0x12345678.
 * Beware: a uint8_t promotes to plain int before a shift, so
 * shifting a byte like 0xFF up by 24 overflows int — undefined
 * behaviour. Cast to uint32_t BEFORE shifting. */
uint32_t load_le32(const uint8_t *p);

/* Store v little-endian into p[0]..p[3]; the inverse of load_le32. */
void store_le32(uint8_t *p, uint32_t v);

/* How many of the first n elements lie in [lo, hi] inclusive.
 * n may be 0 (a may then be NULL). Note the return type: a count of
 * array elements is a size_t, never an int. */
size_t count_in_range(const int32_t *a, size_t n, int32_t lo, int32_t hi);

/* True iff v is representable as an int16_t, i.e. lies within
 * [INT16_MIN, INT16_MAX]. */
bool fits_int16(long v);
```

The harness round-trips `store_le32`/`load_le32` on several values,
then rebuilds everything with AddressSanitizer and UBSan and runs it
again: reading or writing outside the given buffer, or a shift that
overflows `int`, fails the task even when the value comes out right.

Grade with `ctrain check fixed-width-bytes`.
