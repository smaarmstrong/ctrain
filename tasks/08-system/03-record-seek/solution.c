#define _POSIX_C_SOURCE 200809L
#include <sys/types.h>
#include <unistd.h>

enum { REC_SIZE = 16 };

static int seek_to(int fd, long idx)
{
    if (idx < 0)
        return -1;
    if (lseek(fd, (off_t)idx * REC_SIZE, SEEK_SET) == (off_t)-1)
        return -1;
    return 0;
}

int record_read(int fd, long idx, unsigned char *out)
{
    if (seek_to(fd, idx) < 0)
        return -1;

    size_t done = 0;
    while (done < REC_SIZE) {
        ssize_t n = read(fd, out + done, REC_SIZE - done);
        if (n <= 0) /* error, or EOF before the record is complete */
            return -1;
        done += (size_t)n;
    }
    return 0;
}

int record_write(int fd, long idx, const unsigned char *rec)
{
    if (seek_to(fd, idx) < 0)
        return -1;

    size_t done = 0;
    while (done < REC_SIZE) {
        ssize_t n = write(fd, rec + done, REC_SIZE - done);
        if (n < 0)
            return -1;
        done += (size_t)n;
    }
    return 0;
}

long record_count(int fd)
{
    off_t end = lseek(fd, 0, SEEK_END);
    if (end == (off_t)-1)
        return -1;
    return (long)(end / REC_SIZE);
}
