#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

/* Copy everything from fd `in` to fd 1. Returns 0 on success, -1 on a
 * read or write error. Loops on short writes. */
static int copy_fd(int in)
{
    char buf[4096];
    ssize_t n;

    while ((n = read(in, buf, sizeof buf)) > 0) {
        ssize_t off = 0;
        while (off < n) {
            ssize_t w = write(1, buf + off, (size_t)(n - off));
            if (w < 0)
                return -1;
            off += w;
        }
    }
    return n < 0 ? -1 : 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        return copy_fd(0) < 0 ? 1 : 0;

    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
            static const char pre[] = "fd-cat: cannot open ";
            write(2, pre, sizeof pre - 1);
            write(2, argv[i], strlen(argv[i]));
            write(2, "\n", 1);
            return 1;
        }
        if (copy_fd(fd) < 0) {
            close(fd);
            return 1;
        }
        close(fd);
    }
    return 0;
}
