#include <stddef.h>
#include <stdio.h>

size_t count_at_least(const int *a, size_t len, int cutoff)
{
    size_t hits = 0;
    for (size_t i = 0; i < len; i++) {
        if (a[i] >= cutoff)
            hits++;
    }
    return hits;
}

size_t count_newlines(FILE *fp)
{
    size_t lines = 0;
    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n')
            lines++;
    }
    return lines;
}
