#include <stddef.h>

/* Walk the bytes yourself — <string.h> is off limits for this task. */

size_t str_length(const char *s)
{
    /* TODO: count characters up to (not including) the '\0'. */
    (void)s;
    return 0;
}

char *str_copy(char *dst, const char *src)
{
    /* TODO: copy every byte of src including the terminator. */
    (void)src;
    return dst;
}

int str_compare(const char *a, const char *b)
{
    /* TODO: negative / zero / positive, comparing unsigned chars. */
    (void)a;
    (void)b;
    return 0;
}
