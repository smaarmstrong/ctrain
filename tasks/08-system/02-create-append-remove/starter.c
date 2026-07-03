#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>

int file_create(const char *path)
{
    /* TODO: open with O_WRONLY | O_CREAT | O_TRUNC, mode 0644; close. */
    (void)path;
    return -1;
}

int file_append(const char *path, const void *buf, size_t n)
{
    /* TODO: open with O_WRONLY | O_APPEND (no O_CREAT); write all n
     * bytes, looping on short writes; close. */
    (void)path;
    (void)buf;
    (void)n;
    return -1;
}

int file_remove(const char *path)
{
    /* TODO: unlink. */
    (void)path;
    return -1;
}
