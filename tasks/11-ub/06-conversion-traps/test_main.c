/* Test harness — provides main() and calls the learner's
 * count_at_least() and count_newlines().
 *
 * count_at_least is fed a len == 0 call whose pointer targets an
 * exactly-sized heap block, so a wrapped loop bound reads past the
 * allocation on the very first iteration under ASan.
 * count_newlines is fed streams that contain 0xFF bytes: mistaking
 * them for EOF undercounts; mistaking EOF for a byte never stops. */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t count_at_least(const int *a, size_t len, int cutoff);
size_t count_newlines(FILE *fp);

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

/* Open a temporary stream holding exactly the given bytes, rewound. */
static FILE *stream_of(const unsigned char *bytes, size_t n)
{
    FILE *fp = tmpfile();
    if (!fp) {
        fprintf(stderr, "harness: tmpfile failed\n");
        exit(2);
    }
    if (n > 0 && fwrite(bytes, 1, n, fp) != n) {
        fprintf(stderr, "harness: fwrite failed\n");
        exit(2);
    }
    rewind(fp);
    return fp;
}

int main(void)
{
    /* --- count_at_least: ordinary cases ------------------------------- */
    {
        const int a[] = {5, 9, 2};
        CHECK(count_at_least(a, 3, 5) == 2, "count_at_least {5,9,2} cutoff 5 -> 2");
        CHECK(count_at_least(a, 3, 10) == 0, "count_at_least nothing reaches cutoff -> 0");
        CHECK(count_at_least(a, 1, 0) == 1, "count_at_least len 1 -> 1");
    }
    {
        const int a[] = {-4, -4, 7, 0};
        CHECK(count_at_least(a, 4, -4) == 4, "count_at_least negative cutoff counts all");
    }

    /* --- count_at_least: len == 0 must read nothing -------------------- */
    {
        int *heap = malloc(2 * sizeof *heap); /* exactly-sized: any read
                                                 beyond trips ASan */
        if (!heap)
            return 2;
        heap[0] = 100;
        heap[1] = 100;
        CHECK(count_at_least(heap + 2, 0, 0) == 0,
              "count_at_least len 0 returns 0 without reading");
        free(heap);
    }

    /* --- count_newlines: plain text ------------------------------------ */
    {
        const unsigned char text[] = "one\ntwo\nthree\n";
        FILE *fp = stream_of(text, sizeof text - 1);
        CHECK(count_newlines(fp) == 3, "count_newlines three plain lines -> 3");
        fclose(fp);
    }
    {
        FILE *fp = stream_of(NULL, 0);
        CHECK(count_newlines(fp) == 0, "count_newlines empty stream -> 0");
        fclose(fp);
    }
    {
        const unsigned char text[] = "no trailing newline";
        FILE *fp = stream_of(text, sizeof text - 1);
        CHECK(count_newlines(fp) == 0, "count_newlines unterminated line -> 0");
        fclose(fp);
    }

    /* --- count_newlines: 0xFF bytes are data, not end-of-file ---------- */
    {
        const unsigned char bytes[] = {'a', '\n', 0xFF, 0xFF, 'b', '\n', 'c', '\n'};
        FILE *fp = stream_of(bytes, sizeof bytes);
        CHECK(count_newlines(fp) == 3, "count_newlines reads past 0xFF bytes -> 3");
        fclose(fp);
    }
    {
        const unsigned char bytes[] = {0xFF, '\n', 0x00, '\n'};
        FILE *fp = stream_of(bytes, sizeof bytes);
        CHECK(count_newlines(fp) == 2, "count_newlines handles 0xFF and NUL bytes -> 2");
        fclose(fp);
    }

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
