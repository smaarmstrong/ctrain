/* Test harness — provides main() and calls the learner's functions. */
#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int file_create(const char *path);
int file_append(const char *path, const void *buf, size_t n);
int file_remove(const char *path);

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

static long fsize(const char *p)
{
    struct stat st;
    return stat(p, &st) == 0 ? (long)st.st_size : -1;
}

int main(void)
{
    umask(0); /* so the mode passed to open() is exactly what sticks */

    char dir[] = "/tmp/ctrain-car.XXXXXX";
    if (!mkdtemp(dir)) {
        perror("mkdtemp");
        return 2;
    }
    char path[sizeof dir + 16], ghost[sizeof dir + 16];
    snprintf(path, sizeof path, "%s/f.dat", dir);
    snprintf(ghost, sizeof ghost, "%s/ghost.dat", dir);

    /* create */
    CHECK(file_create(path) == 0, "file_create returns 0");
    struct stat st;
    CHECK(stat(path, &st) == 0 && S_ISREG(st.st_mode),
          "the file exists and is a regular file");
    CHECK(fsize(path) == 0, "a freshly created file is empty");
    if (stat(path, &st) == 0)
        CHECK((st.st_mode & 07777) == 0644, "permission bits are exactly 0644");
    else
        CHECK(0, "permission bits are exactly 0644");

    /* append, twice, including raw bytes */
    CHECK(file_append(path, "hello ", 6) == 0, "first append returns 0");
    CHECK(file_append(path, "world\0\xff!", 8) == 0,
          "second append (with NUL and high bytes) returns 0");
    CHECK(fsize(path) == 14, "file size is the sum of appended byte counts");

    unsigned char got[32] = {0};
    int fd = open(path, O_RDONLY);
    ssize_t n = fd >= 0 ? read(fd, got, sizeof got) : -1;
    if (fd >= 0)
        close(fd);
    CHECK(n == 14 && memcmp(got, "hello world\0\xff!", 14) == 0,
          "the appended bytes land in order, byte-exact");

    CHECK(file_append(path, "", 0) == 0, "appending 0 bytes succeeds");
    CHECK(fsize(path) == 14, "...and changes nothing");

    /* create truncates */
    CHECK(file_create(path) == 0, "file_create on an existing file returns 0");
    CHECK(fsize(path) == 0, "...and truncates it to zero bytes");

    /* append must not create */
    CHECK(file_append(ghost, "x", 1) == -1,
          "append to a missing file returns -1");
    CHECK(stat(ghost, &st) != 0, "...and does not create it");

    /* remove */
    CHECK(file_remove(path) == 0, "file_remove returns 0");
    CHECK(stat(path, &st) != 0, "...and the file is gone");
    CHECK(file_remove(path) == -1, "removing it a second time returns -1");

    rmdir(dir);

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
