#include <stdint.h>

int popcount32(uint32_t x)
{
    int count = 0;
    while (x != 0) {
        x &= x - 1; /* clear the lowest set bit */
        count++;
    }
    return count;
}

int trailing_zeros32(uint32_t x)
{
    if (x == 0)
        return 32;
    int count = 0;
    while ((x & 1u) == 0) {
        x >>= 1;
        count++;
    }
    return count;
}

uint32_t rotl32(uint32_t x, unsigned k)
{
    k %= 32;
    if (k == 0)
        return x;
    return (x << k) | (x >> (32 - k));
}

uint32_t reverse_bits32(uint32_t x)
{
    uint32_t out = 0;
    for (int i = 0; i < 32; i++) {
        out = (out << 1) | (x & 1u);
        x >>= 1;
    }
    return out;
}
