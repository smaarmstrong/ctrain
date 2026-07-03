#include <stddef.h>

/* Copy src into dst, never writing more than dst_cap bytes total.
 * See the prompt for the full contract.
 *
 * Somewhere below, one byte escapes the buffer. Find it, fix it. */
size_t bounded_copy(char *dst, size_t dst_cap, const char *src)
{
    size_t i = 0;
    while (src[i] != '\0' && i < dst_cap) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
    return i;
}
