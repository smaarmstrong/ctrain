#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>

int file_create(const char *path)
{
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        return -1;
    return close(fd) == 0 ? 0 : -1;
}

int file_append(const char *path, const void *buf, size_t n)
{
    int fd = open(path, O_WRONLY | O_APPEND);
    if (fd < 0)
        return -1;

    const unsigned char *p = buf;
    size_t done = 0;
    while (done < n) {
        ssize_t w = write(fd, p + done, n - done);
        if (w < 0) {
            close(fd);
            return -1;
        }
        done += (size_t)w;
    }
    return close(fd) == 0 ? 0 : -1;
}

int file_remove(const char *path)
{
    return unlink(path) == 0 ? 0 : -1;
}
