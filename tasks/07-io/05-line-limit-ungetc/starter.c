#include <ctype.h>
#include <stddef.h>
#include <stdio.h>

long line_limited(FILE *stream, char *buf, size_t cap)
{
    /* TODO: read up to cap-1 chars (stop after '\n', keep it),
     * NUL-terminate, return chars stored, -1 at end-of-file.
     * A too-long line is split: leave the rest in the stream. */
    (void)stream;
    (void)cap;
    buf[0] = '\0';
    return -1;
}

int next_int(FILE *stream, int *out)
{
    /* TODO: skip whitespace, accept an optional sign, read digits,
     * push the first non-digit back with ungetc.
     * Return 1 = got a value, 0 = not a number, -1 = end-of-file. */
    (void)stream;
    (void)out;
    return -1;
}
