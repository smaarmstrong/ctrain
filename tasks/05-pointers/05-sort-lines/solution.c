#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Read one line of any length. Returns a malloc'd string without the
 * '\n', or NULL at EOF when nothing was read. */
static char *read_line(void)
{
    size_t cap = 16, len = 0;
    char *buf = malloc(cap);
    if (!buf)
        exit(1);
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (len + 1 >= cap) {
            cap *= 2;
            char *nb = realloc(buf, cap);
            if (!nb)
                exit(1);
            buf = nb;
        }
        buf[len++] = (char)c;
    }
    if (c == EOF && len == 0) {
        free(buf);
        return NULL;
    }
    buf[len] = '\0';
    return buf;
}

static int cmp_lines(const void *a, const void *b)
{
    const char *const *pa = a;
    const char *const *pb = b;
    return strcmp(*pa, *pb);
}

int main(void)
{
    size_t cap = 8, n = 0;
    char **lines = malloc(cap * sizeof *lines);
    if (!lines)
        return 1;

    char *line;
    while ((line = read_line()) != NULL) {
        if (n == cap) {
            cap *= 2;
            char **nl = realloc(lines, cap * sizeof *nl);
            if (!nl)
                return 1;
            lines = nl;
        }
        lines[n++] = line;
    }

    qsort(lines, n, sizeof *lines, cmp_lines);

    for (size_t i = 0; i < n; i++) {
        puts(lines[i]);
        free(lines[i]);
    }
    free(lines);
    return 0;
}
