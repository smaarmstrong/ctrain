#include <stdbool.h>
#include <stddef.h>

int digit_value(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    return -1;
}

int hex_value(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}

char flip_case(char c)
{
    if (c >= 'a' && c <= 'z')
        return (char)(c - 'a' + 'A');
    if (c >= 'A' && c <= 'Z')
        return (char)(c - 'A' + 'a');
    return c;
}

bool parse_decimal(const char *s, long *out)
{
    if (s == NULL || *s == '\0')
        return false;
    bool negative = false;
    if (*s == '-') {
        negative = true;
        s++;
        if (*s == '\0')
            return false;
    }
    long value = 0;
    for (; *s != '\0'; s++) {
        int d = digit_value(*s);
        if (d < 0)
            return false;
        value = value * 10 + d;
    }
    *out = negative ? -value : value;
    return true;
}
