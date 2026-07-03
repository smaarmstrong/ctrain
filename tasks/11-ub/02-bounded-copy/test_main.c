/* Test harness — provides main() and calls the learner's bounded_copy().
 * Stack buffers carry sentinel bytes just past the advertised capacity;
 * heap buffers are exactly capacity-sized so ASan sees any overflow. */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t bounded_copy(char *dst, size_t dst_cap, const char *src);

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

int main(void)
{
    /* fits comfortably */
    {
        char buf[12];
        memset(buf, 'X', sizeof buf);
        size_t n = bounded_copy(buf, 10, "hi");
        CHECK(n == 2 && strcmp(buf, "hi") == 0, "short string copied intact, returns 2");
        CHECK(buf[10] == 'X' && buf[11] == 'X', "no write past capacity on the easy case");
    }

    /* exact fit: strlen(src) == dst_cap - 1 */
    {
        char buf[7];
        memset(buf, 'X', sizeof buf);
        size_t n = bounded_copy(buf, 6, "hello");
        CHECK(n == 5 && strcmp(buf, "hello") == 0, "exact-fit copy, returns 5");
        CHECK(buf[6] == 'X', "sentinel after an exact-fit copy untouched");
    }

    /* truncation: src longer than the buffer */
    {
        char buf[6];
        memset(buf, 'X', sizeof buf);
        size_t n = bounded_copy(buf, 4, "hello");
        CHECK(n == 3, "truncated copy returns 3");
        CHECK(strcmp(buf, "hel") == 0, "truncated result is \"hel\" with terminator");
        CHECK(buf[4] == 'X' && buf[5] == 'X', "sentinels after a truncated copy untouched");
    }

    /* capacity 1: room for the terminator only */
    {
        char buf[3];
        memset(buf, 'X', sizeof buf);
        size_t n = bounded_copy(buf, 1, "hello");
        CHECK(n == 0 && buf[0] == '\0', "cap 1 stores only the terminator, returns 0");
        CHECK(buf[1] == 'X' && buf[2] == 'X', "sentinels after a cap-1 copy untouched");
    }

    /* capacity 0: must not touch dst at all */
    {
        char buf[2];
        memset(buf, 'X', sizeof buf);
        size_t n = bounded_copy(buf, 0, "hello");
        CHECK(n == 0, "cap 0 returns 0");
        CHECK(buf[0] == 'X' && buf[1] == 'X', "cap 0 writes nothing at all");
    }

    /* empty source */
    {
        char buf[3];
        memset(buf, 'X', sizeof buf);
        size_t n = bounded_copy(buf, 3, "");
        CHECK(n == 0 && buf[0] == '\0', "empty source stores just the terminator");
        CHECK(buf[1] == 'X' && buf[2] == 'X', "sentinels after an empty-source copy untouched");
    }

    /* exactly-sized heap buffers: under ASan a single stray byte aborts */
    {
        char *h = malloc(4);
        if (!h)
            return 2;
        size_t n = bounded_copy(h, 4, "overflow me");
        CHECK(n == 3 && strcmp(h, "ove") == 0, "heap truncation, exact-size allocation");
        free(h);

        h = malloc(1);
        if (!h)
            return 2;
        n = bounded_copy(h, 1, "x");
        CHECK(n == 0 && h[0] == '\0', "heap cap-1 buffer holds only the terminator");
        free(h);
    }

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
