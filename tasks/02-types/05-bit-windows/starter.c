unsigned get_bits(unsigned x, int pos, int n)
{
    /* TODO: bits pos .. pos+n-1, shifted down to bit 0. */
    (void)x;
    (void)pos;
    (void)n;
    return 0;
}

unsigned set_bits(unsigned x, int pos, int n, unsigned v)
{
    /* TODO: clear the field in x, then drop in the low n bits of v. */
    (void)pos;
    (void)n;
    (void)v;
    return x;
}

unsigned toggle_bits(unsigned x, int pos, int n)
{
    /* TODO: XOR with a mask covering the field. */
    (void)pos;
    (void)n;
    return x;
}
