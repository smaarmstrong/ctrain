#include <stdbool.h>
#include <stddef.h>

int digit_value(char c)
{
    /* TODO: '0'..'9' -> 0..9, else -1. */
    (void)c;
    return -1;
}

int hex_value(char c)
{
    /* TODO: 0..15 for hex digits (either case), else -1. */
    (void)c;
    return -1;
}

char flip_case(char c)
{
    /* TODO: swap the case of ASCII letters, pass others through. */
    return c;
}

bool parse_decimal(const char *s, long *out)
{
    /* TODO: optional '-', then digits only; true + *out on success. */
    (void)s;
    (void)out;
    return false;
}
