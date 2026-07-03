#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

uint16_t load_le16(const uint8_t *p)
{
    /* TODO: p[0] is the low byte, p[1] the high byte. */
    (void)p;
    return 0;
}

uint32_t load_le32(const uint8_t *p)
{
    /* TODO: four bytes, least significant first — cast each byte to
     * uint32_t before shifting it into place. */
    (void)p;
    return 0;
}

void store_le32(uint8_t *p, uint32_t v)
{
    /* TODO: the inverse of load_le32. */
    (void)p;
    (void)v;
}

size_t count_in_range(const int32_t *a, size_t n, int32_t lo, int32_t hi)
{
    /* TODO: how many of a[0..n-1] satisfy lo <= a[i] <= hi. */
    (void)a;
    (void)n;
    (void)lo;
    (void)hi;
    return 0;
}

bool fits_int16(long v)
{
    /* TODO: <stdint.h> has the limits ready-made. */
    (void)v;
    return false;
}
