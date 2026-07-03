#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

uint16_t load_le16(const uint8_t *p)
{
    return (uint16_t)(p[0] | ((uint16_t)p[1] << 8));
}

uint32_t load_le32(const uint8_t *p)
{
    return (uint32_t)p[0]
         | ((uint32_t)p[1] << 8)
         | ((uint32_t)p[2] << 16)
         | ((uint32_t)p[3] << 24);
}

void store_le32(uint8_t *p, uint32_t v)
{
    p[0] = (uint8_t)(v & 0xFFu);
    p[1] = (uint8_t)((v >> 8) & 0xFFu);
    p[2] = (uint8_t)((v >> 16) & 0xFFu);
    p[3] = (uint8_t)((v >> 24) & 0xFFu);
}

size_t count_in_range(const int32_t *a, size_t n, int32_t lo, int32_t hi)
{
    size_t count = 0;
    for (size_t i = 0; i < n; i++)
        if (a[i] >= lo && a[i] <= hi)
            count++;
    return count;
}

bool fits_int16(long v)
{
    return v >= INT16_MIN && v <= INT16_MAX;
}
