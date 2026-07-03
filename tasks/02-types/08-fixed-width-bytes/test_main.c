/* Test harness — provides main() and calls the learner's functions. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint16_t load_le16(const uint8_t *p);
uint32_t load_le32(const uint8_t *p);
void store_le32(uint8_t *p, uint32_t v);
size_t count_in_range(const int32_t *a, size_t n, int32_t lo, int32_t hi);
bool fits_int16(long v);

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
    /* load_le16 — exact-size heap buffers so ASan catches overreads */
    uint8_t *b2 = malloc(2);
    if (!b2)
        return 2;
    b2[0] = 0x34; b2[1] = 0x12;
    CHECK(load_le16(b2) == 0x1234u, "load_le16({34,12}) == 0x1234");
    b2[0] = 0xFF; b2[1] = 0xFF;
    CHECK(load_le16(b2) == 0xFFFFu, "load_le16 all-ones");
    b2[0] = 0x00; b2[1] = 0x80;
    CHECK(load_le16(b2) == 0x8000u, "load_le16 top bit");
    free(b2);

    /* load_le32 */
    uint8_t *b4 = malloc(4);
    if (!b4)
        return 2;
    b4[0] = 0x78; b4[1] = 0x56; b4[2] = 0x34; b4[3] = 0x12;
    CHECK(load_le32(b4) == 0x12345678u, "load_le32({78,56,34,12}) == 0x12345678");
    memset(b4, 0xFF, 4);
    CHECK(load_le32(b4) == 0xFFFFFFFFu,
          "load_le32 all-ones (byte<<24 must not overflow int)");
    memset(b4, 0, 4);
    b4[3] = 0x80;
    CHECK(load_le32(b4) == 0x80000000u, "load_le32 top bit only");

    /* store_le32 — bytes land in the right cells */
    store_le32(b4, 0xAABBCCDDu);
    CHECK(b4[0] == 0xDD && b4[1] == 0xCC && b4[2] == 0xBB && b4[3] == 0xAA,
          "store_le32 writes least significant byte first");
    store_le32(b4, 0);
    CHECK(b4[0] == 0 && b4[1] == 0 && b4[2] == 0 && b4[3] == 0,
          "store_le32(_, 0) zeroes all four bytes");

    /* round trips */
    const uint32_t samples[] = {0u, 1u, 0xDEADBEEFu, 0x80000000u, 0xFFFFFFFFu};
    bool all_round_trip = true;
    for (size_t i = 0; i < sizeof samples / sizeof samples[0]; i++) {
        store_le32(b4, samples[i]);
        if (load_le32(b4) != samples[i])
            all_round_trip = false;
    }
    CHECK(all_round_trip, "store_le32 then load_le32 round-trips");
    free(b4);

    /* count_in_range */
    int32_t vals[] = {-5, 0, 3, 7, 7, 100, INT32_MIN, INT32_MAX};
    CHECK(count_in_range(vals, 8, 0, 10) == 4, "four values in [0,10]");
    CHECK(count_in_range(vals, 8, 7, 7) == 2, "inclusive single-point range");
    CHECK(count_in_range(vals, 8, INT32_MIN, INT32_MAX) == 8,
          "everything is in the full range");
    CHECK(count_in_range(vals, 3, 0, 10) == 2, "only the first n are counted");
    CHECK(count_in_range(NULL, 0, 0, 10) == 0, "n == 0 counts nothing");

    /* fits_int16 */
    CHECK(fits_int16(0) == true, "0 fits");
    CHECK(fits_int16(32767) == true, "INT16_MAX fits");
    CHECK(fits_int16(32768) == false, "INT16_MAX + 1 does not fit");
    CHECK(fits_int16(-32768) == true, "INT16_MIN fits");
    CHECK(fits_int16(-32769) == false, "INT16_MIN - 1 does not fit");
    CHECK(fits_int16(1000000L) == false, "a million does not fit");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
