#include <stddef.h>
#include <string.h>

size_t count_occurrences(const char *hay, const char *needle)
{
    /* TODO: strstr in a loop; resume past each whole match. */
    (void)hay;
    (void)needle;
    return 0;
}

size_t split_fields(char *line, char *fields[], size_t max)
{
    /* TODO: strtok(line, ",") then strtok(NULL, ",") — stop at max. */
    (void)line;
    (void)fields;
    (void)max;
    return 0;
}

void erase_range(char *s, size_t pos, size_t count)
{
    /* TODO: clamp count, then move the tail (and its NUL) left.
     * The source and destination overlap. */
    (void)s;
    (void)pos;
    (void)count;
}
