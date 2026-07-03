/* Test harness — provides main() and intercepts read() via -Wl,--wrap=read
 * to count how many syscalls the learner's reader issues. */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

struct breader;
struct breader *br_open(int fd);
int br_getc(struct breader *b);
int br_ungetc(struct breader *b, int c);
void br_close(struct breader *b);

/* ---- read() interception ------------------------------------------------ */
static long read_calls;
ssize_t __real_read(int fd, void *buf, size_t n);
ssize_t __wrap_read(int fd, void *buf, size_t n)
{
    read_calls++;
    return __real_read(fd, buf, n);
}

/* ---- checks -------------------------------------------------------------- */
static int failures;

#define CHECK(cond, msg)                                        \
    do {                                                        \
        if (cond) {                                             \
            printf("  ok    %s\n", msg);                        \
        } else {                                                \
            printf("  FAIL  %s\n", msg);                        \
            failures++;                                         \
        }                                                       \
    } while (0)

enum { BIG = 8192 };

static unsigned char pat(long i)
{
    return (unsigned char)((i * 31 + 7) & 0xFF); /* covers 0..255, incl. >=128 */
}

/* Build an unlinked temp file of `size` patterned bytes; fd at offset 0. */
static int make_file(long size)
{
    char tmpl[] = "/tmp/ctrain-br.XXXXXX";
    int fd = mkstemp(tmpl);
    if (fd < 0)
        return -1;
    unlink(tmpl);

    unsigned char buf[1024];
    long done = 0;
    while (done < size) {
        long chunk = size - done > 1024 ? 1024 : size - done;
        for (long i = 0; i < chunk; i++)
            buf[i] = pat(done + i);
        if (write(fd, buf, (size_t)chunk) != chunk)
            return -1;
        done += chunk;
    }
    if (lseek(fd, 0, SEEK_SET) == (off_t)-1)
        return -1;
    return fd;
}

int main(void)
{
    /* --- phase 1: a full 8 KiB read, counting syscalls ------------------- */
    int fd = make_file(BIG);
    if (fd < 0) {
        perror("harness: make_file");
        return 2;
    }
    read_calls = 0;
    struct breader *b = br_open(fd);
    CHECK(b != NULL, "br_open returns reader state");
    if (!b)
        return 1;

    long mismatches = 0;
    for (long i = 0; i < BIG; i++)
        if (br_getc(b) != (int)pat(i))
            mismatches++;
    CHECK(mismatches == 0, "all 8192 bytes come back intact and in order");
    CHECK(br_getc(b) == -1, "end of input returns -1");
    CHECK(br_getc(b) == -1 && br_getc(b) == -1, "…and keeps returning -1");
    CHECK(read_calls >= 1, "data was fetched with the read() syscall");
    CHECK(read_calls <= 24, "8 KiB took at most 24 read() calls (buffered)");
    if (read_calls > 24)
        printf("        (your reader made %ld read() calls)\n", read_calls);
    br_close(b);
    close(fd);

    /* --- phase 2: pushback ------------------------------------------------ */
    fd = make_file(10);
    if (fd < 0)
        return 2;
    b = br_open(fd);
    CHECK(b != NULL, "br_open returns reader state for a small file");
    if (!b)
        return 1;

    CHECK(br_getc(b) == (int)pat(0), "first byte is correct");
    CHECK(br_ungetc(b, 'Z') == 'Z', "br_ungetc accepts one byte");
    CHECK(br_ungetc(b, 'Y') == -1, "a second push while one is pending fails");
    CHECK(br_getc(b) == 'Z', "the pushed-back byte comes out next");
    CHECK(br_getc(b) == (int)pat(1), "then reading resumes where it left off");
    CHECK(br_ungetc(b, -1) == -1, "br_ungetc rejects a value outside 0..255");
    CHECK(br_ungetc(b, 256) == -1, "br_ungetc rejects 256");
    for (long i = 2; i < 10; i++)
        (void)br_getc(b);
    CHECK(br_getc(b) == -1, "the small file ends with -1");
    CHECK(br_ungetc(b, 'Q') == 'Q', "pushback still works after end of input");
    CHECK(br_getc(b) == 'Q', "the byte pushed after EOF is returned");
    CHECK(br_getc(b) == -1, "then it is end of input again");
    br_close(b);
    close(fd);

    /* --- phase 3: empty file ---------------------------------------------- */
    fd = make_file(0);
    if (fd < 0)
        return 2;
    b = br_open(fd);
    CHECK(b != NULL, "br_open works on an empty file");
    if (b) {
        CHECK(br_getc(b) == -1, "an empty file returns -1 immediately");
        br_close(b);
    } else {
        failures++;
    }
    close(fd);

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
