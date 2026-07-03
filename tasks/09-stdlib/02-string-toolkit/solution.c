#include <stddef.h>
#include <string.h>

size_t count_occurrences(const char *hay, const char *needle)
{
    size_t nlen = strlen(needle);
    if (nlen == 0)
        return 0;

    size_t n = 0;
    const char *p = hay;
    while ((p = strstr(p, needle)) != NULL) {
        n++;
        p += nlen;
    }
    return n;
}

size_t split_fields(char *line, char *fields[], size_t max)
{
    size_t n = 0;
    for (char *tok = strtok(line, ","); tok != NULL && n < max;
         tok = strtok(NULL, ","))
        fields[n++] = tok;
    return n;
}

void erase_range(char *s, size_t pos, size_t count)
{
    size_t len = strlen(s);
    if (pos > len)
        return;
    if (count > len - pos)
        count = len - pos;
    /* +1 drags the NUL terminator along; regions overlap -> memmove. */
    memmove(s + pos, s + pos + count, len - pos - count + 1);
}
