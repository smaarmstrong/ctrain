#include <stdbool.h>

/* Each body below shows — with explicit parentheses — how C actually
 * grouped the original author's unparenthesised expression. All four
 * are wrong. Fix them to match the specs in the prompt. */

bool between(int lo, int x, int hi)
{
    /* was: lo <= x <= hi */
    return (lo <= x) <= hi;
}

bool low_nibble_is(unsigned x, unsigned v)
{
    /* was: x & 0xF == v */
    return x & (0xFu == v);
}

int shifted_sum(int a, int b)
{
    /* was: a << 2 + b */
    return a << (2 + b);
}

int bonus_total(int base, bool bonus)
{
    /* was: base + bonus ? 10 : 0 */
    return (base + bonus) ? 10 : 0;
}
