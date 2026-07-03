#define _POSIX_C_SOURCE 200809L
#include <unistd.h>

enum { REC_SIZE = 16 };

int record_read(int fd, long idx, unsigned char *out)
{
    /* TODO: lseek to idx * REC_SIZE, then read exactly 16 bytes,
     * looping on short reads; -1 on negative idx, error, or EOF
     * mid-record. */
    (void)fd;
    (void)idx;
    (void)out;
    return -1;
}

int record_write(int fd, long idx, const unsigned char *rec)
{
    /* TODO: lseek, then write exactly 16 bytes (loop on short
     * writes). */
    (void)fd;
    (void)idx;
    (void)rec;
    return -1;
}

long record_count(int fd)
{
    /* TODO: file size via lseek(fd, 0, SEEK_END), divided by 16. */
    (void)fd;
    return -1;
}
