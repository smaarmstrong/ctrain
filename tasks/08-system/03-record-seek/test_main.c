/* Test harness — provides main() and calls the learner's functions. */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

enum { REC = 16 };

int record_read(int fd, long idx, unsigned char *out);
int record_write(int fd, long idx, const unsigned char *rec);
long record_count(int fd);

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

static void fill(unsigned char *r, int v)
{
    memset(r, v, REC);
}

int main(void)
{
    char tmpl[] = "/tmp/ctrain-rec.XXXXXX";
    int fd = mkstemp(tmpl);
    if (fd < 0) {
        perror("mkstemp");
        return 2;
    }
    unlink(tmpl); /* anonymous once closed */

    unsigned char rec[REC], want[REC], got[REC];

    CHECK(record_count(fd) == 0, "an empty file holds zero records");

    /* write five records deliberately out of order */
    static const int worder[] = {3, 0, 2, 1, 4};
    int ok = 1;
    for (int i = 0; i < 5; i++) {
        fill(rec, 'A' + worder[i]);
        if (record_write(fd, worder[i], rec) != 0)
            ok = 0;
    }
    CHECK(ok, "five records written out of order all return 0");
    CHECK(record_count(fd) == 5, "record_count now reports 5");

    /* read them back in a different order */
    static const int rorder[] = {4, 1, 3, 0, 2};
    ok = 1;
    for (int i = 0; i < 5; i++) {
        fill(want, 'A' + rorder[i]);
        memset(got, 0, REC);
        if (record_read(fd, rorder[i], got) != 0 ||
            memcmp(got, want, REC) != 0)
            ok = 0;
    }
    CHECK(ok, "records read back out of order match what was written");

    /* overwrite the middle record; neighbours must be untouched */
    fill(rec, 'Z');
    CHECK(record_write(fd, 2, rec) == 0, "overwriting record 2 returns 0");
    memset(got, 0, REC);
    CHECK(record_read(fd, 2, got) == 0 && memcmp(got, rec, REC) == 0,
          "record 2 now holds the new bytes");
    fill(want, 'A' + 1);
    memset(got, 0, REC);
    CHECK(record_read(fd, 1, got) == 0 && memcmp(got, want, REC) == 0,
          "record 1 is untouched");
    fill(want, 'A' + 3);
    memset(got, 0, REC);
    CHECK(record_read(fd, 3, got) == 0 && memcmp(got, want, REC) == 0,
          "record 3 is untouched");
    CHECK(record_count(fd) == 5, "overwriting does not change the count");

    /* bad indices */
    CHECK(record_read(fd, 5, got) == -1, "reading one past the end returns -1");
    CHECK(record_read(fd, -1, got) == -1, "reading a negative index returns -1");
    CHECK(record_write(fd, -1, rec) == -1, "writing a negative index returns -1");

    /* writing beyond EOF extends the file; the gap reads as zeros */
    fill(rec, 'H');
    CHECK(record_write(fd, 8, rec) == 0, "writing record 8 beyond EOF returns 0");
    CHECK(record_count(fd) == 9, "the file now holds 9 records");
    memset(got, 0xEE, REC);
    memset(want, 0, REC);
    CHECK(record_read(fd, 6, got) == 0 && memcmp(got, want, REC) == 0,
          "the gap record 6 reads back as 16 zero bytes");
    fill(want, 'H');
    memset(got, 0, REC);
    CHECK(record_read(fd, 8, got) == 0 && memcmp(got, want, REC) == 0,
          "record 8 holds what was written");

    /* a trailing partial record never counts */
    if (lseek(fd, 0, SEEK_END) == (off_t)-1 || write(fd, "12345", 5) != 5) {
        fprintf(stderr, "harness: could not append partial tail\n");
        return 2;
    }
    CHECK(record_count(fd) == 9, "a 5-byte partial tail does not count");
    CHECK(record_read(fd, 9, got) == -1,
          "reading the partial record 9 returns -1");

    close(fd);

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
