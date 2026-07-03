#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <unistd.h>

/* TODO: state = fd, a buffer of >= 512 bytes, fill level, read
 * position, an end-of-input flag, and the pending pushed-back byte
 * (or -1 for none). */
struct breader {
    int fd;
};

struct breader *br_open(int fd)
{
    /* TODO: malloc the state and initialise it. */
    (void)fd;
    return NULL;
}

int br_getc(struct breader *b)
{
    /* TODO: pending byte first; refill with read() when the buffer is
     * empty; -1 forever once input is exhausted. */
    (void)b;
    return -1;
}

int br_ungetc(struct breader *b, int c)
{
    /* TODO: store at most one pending byte; reject c outside 0..255
     * and a second push while one is waiting. */
    (void)b;
    (void)c;
    return -1;
}

void br_close(struct breader *b)
{
    free(b);
}
