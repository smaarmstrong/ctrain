#include <stdbool.h>

bool between(int lo, int x, int hi)
{
    return lo <= x && x <= hi;
}

bool low_nibble_is(unsigned x, unsigned v)
{
    return (x & 0xFu) == v;
}

int shifted_sum(int a, int b)
{
    return (a << 2) + b;
}

int bonus_total(int base, bool bonus)
{
    return base + (bonus ? 10 : 0);
}
