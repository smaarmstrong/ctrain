#include <ctype.h>
#include <stddef.h>
#include <stdio.h>

long line_limited(FILE *stream, char *buf, size_t cap)
{
    size_t n = 0;
    int c = 0;

    while (n + 1 < cap && (c = fgetc(stream)) != EOF) {
        buf[n++] = (char)c;
        if (c == '\n')
            break;
    }
    buf[n] = '\0';
    if (n == 0 && c == EOF)
        return -1;
    return (long)n;
}

int next_int(FILE *stream, int *out)
{
    int c;

    do {
        c = fgetc(stream);
        if (c == EOF)
            return -1;
    } while (isspace((unsigned char)c));

    int sign = 1;
    if (c == '+' || c == '-') {
        sign = (c == '-') ? -1 : 1;
        c = fgetc(stream);
        if (!isdigit((unsigned char)c)) {
            if (c != EOF)
                ungetc(c, stream);
            return 0;
        }
    } else if (!isdigit((unsigned char)c)) {
        ungetc(c, stream);
        return 0;
    }

    int value = 0;
    while (isdigit((unsigned char)c)) {
        value = 10 * value + (c - '0');
        c = fgetc(stream);
    }
    if (c != EOF)
        ungetc(c, stream);
    *out = sign * value;
    return 1;
}
