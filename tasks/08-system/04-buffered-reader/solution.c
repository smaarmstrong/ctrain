#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <unistd.h>

enum { BR_BUFSIZE = 4096 };

struct breader {
    int fd;
    int pending; /* pushed-back byte, or -1 for none */
    int eof;
    size_t pos, len;
    unsigned char buf[BR_BUFSIZE];
};

struct breader *br_open(int fd)
{
    struct breader *b = malloc(sizeof *b);
    if (!b)
        return NULL;
    b->fd = fd;
    b->pending = -1;
    b->eof = 0;
    b->pos = 0;
    b->len = 0;
    return b;
}

int br_getc(struct breader *b)
{
    if (b->pending >= 0) {
        int c = b->pending;
        b->pending = -1;
        return c;
    }
    if (b->pos >= b->len) {
        if (b->eof)
            return -1;
        ssize_t n = read(b->fd, b->buf, BR_BUFSIZE);
        if (n <= 0) {
            b->eof = 1;
            return -1;
        }
        b->len = (size_t)n;
        b->pos = 0;
    }
    return b->buf[b->pos++];
}

int br_ungetc(struct breader *b, int c)
{
    if (c < 0 || c > 255 || b->pending >= 0)
        return -1;
    b->pending = c;
    return c;
}

void br_close(struct breader *b)
{
    free(b);
}
