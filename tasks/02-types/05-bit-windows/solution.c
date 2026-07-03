/* Mask with the low n bits set, valid for 1 <= n <= 32: shifting
 * ~0u right by (32 - n) never needs a shift count of 32. */
static unsigned low_mask(int n)
{
    return ~0u >> (32 - n);
}

unsigned get_bits(unsigned x, int pos, int n)
{
    return (x >> pos) & low_mask(n);
}

unsigned set_bits(unsigned x, int pos, int n, unsigned v)
{
    unsigned field = low_mask(n) << pos;
    return (x & ~field) | ((v & low_mask(n)) << pos);
}

unsigned toggle_bits(unsigned x, int pos, int n)
{
    return x ^ (low_mask(n) << pos);
}
