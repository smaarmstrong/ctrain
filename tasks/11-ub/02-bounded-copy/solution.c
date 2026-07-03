#include <stddef.h>

size_t bounded_copy(char *dst, size_t dst_cap, const char *src)
{
    if (dst_cap == 0)
        return 0;
    size_t i = 0;
    while (i < dst_cap - 1 && src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
    return i;
}
