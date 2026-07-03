#include <stddef.h>
#include <stdio.h>

/* Two integer-conversion bugs hide below. Both functions compile
 * cleanly; both are wrong. See the prompt for the exact contracts. */

/* Return how many of the first len elements of a are >= cutoff. */
size_t count_at_least(const int *a, size_t len, int cutoff)
{
    size_t hits = 0;
    for (size_t i = 0; i <= len - 1; i++) {
        if (a[i] >= cutoff)
            hits++;
    }
    return hits;
}

/* Read fp to end-of-file and return the number of '\n' bytes seen. */
size_t count_newlines(FILE *fp)
{
    size_t lines = 0;
    char c;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n')
            lines++;
    }
    return lines;
}
