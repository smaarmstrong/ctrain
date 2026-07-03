/* Test harness — provides main() and calls the learner's functions. */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long line_limited(FILE *stream, char *buf, size_t cap);
int next_int(FILE *stream, int *out);

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

/* A seekable stream preloaded with s (tmpfile is standard C). */
static FILE *stream_of(const char *s)
{
    FILE *f = tmpfile();
    if (!f || fputs(s, f) == EOF) {
        fprintf(stderr, "harness: cannot set up temp stream\n");
        exit(2);
    }
    rewind(f);
    return f;
}

static void check_line(FILE *f, char *buf, size_t cap,
                       long want_ret, const char *want_buf, const char *msg)
{
    long r = line_limited(f, buf, cap);
    if (r == want_ret && strcmp(buf, want_buf) == 0) {
        printf("  ok    %s\n", msg);
    } else {
        printf("  FAIL  %s (ret %ld want %ld, buf \"%s\" want \"%s\")\n",
               msg, r, want_ret, buf, want_buf);
        failures++;
    }
}

int main(void)
{
    /* ---- line_limited: roomy buffer -------------------------------- */
    {
        char big[64];
        FILE *f = stream_of("hello\nworld\n");
        check_line(f, big, sizeof big, 6, "hello\n",
                   "reads a whole line including the newline");
        check_line(f, big, sizeof big, 6, "world\n",
                   "second call reads the second line");
        check_line(f, big, sizeof big, -1, "",
                   "returns -1 at end-of-file");
        fclose(f);
    }

    /* ---- line_limited: empty line, no trailing newline -------------- */
    {
        char big[64];
        FILE *f = stream_of("\nend");
        check_line(f, big, sizeof big, 1, "\n",
                   "an empty line is just \"\\n\", length 1");
        check_line(f, big, sizeof big, 3, "end",
                   "a final line without newline is returned as-is");
        check_line(f, big, sizeof big, -1, "",
                   "then -1 on the following call");
        fclose(f);
    }

    /* ---- line_limited: long line split across calls, exact-size buf.
     * The buffer is heap-allocated at exactly cap bytes so that any
     * write past cap trips AddressSanitizer in the sanitized run. */
    {
        enum { CAP = 5 };
        char *tight = malloc(CAP);
        if (!tight)
            return 2;
        FILE *f = stream_of("abcdefghij\nk\n");
        check_line(f, tight, CAP, 4, "abcd",
                   "long line: first chunk fills cap-1 chars");
        check_line(f, tight, CAP, 4, "efgh",
                   "long line: continuation, nothing lost");
        check_line(f, tight, CAP, 3, "ij\n",
                   "long line: final chunk ends at the newline");
        check_line(f, tight, CAP, 2, "k\n",
                   "next line unaffected by the earlier split");
        fclose(f);
        free(tight);
    }

    /* ---- next_int: values, lookahead, pushback ----------------------- */
    {
        FILE *f = stream_of("  42x-7");
        int v = 12345;
        CHECK(next_int(f, &v) == 1 && v == 42, "reads 42, stops at 'x'");
        CHECK(fgetc(f) == 'x', "the 'x' was pushed back for the caller");
        CHECK(next_int(f, &v) == 1 && v == -7, "reads a negative number");
        CHECK(next_int(f, &v) == -1, "end-of-file gives -1");
        fclose(f);
    }
    {
        FILE *f = stream_of("+9 -  z 100\n");
        int v = 777;
        CHECK(next_int(f, &v) == 1 && v == 9, "explicit + sign accepted");
        CHECK(next_int(f, &v) == 0 && v == 9,
              "lone sign: returns 0, *out untouched");
        CHECK(fgetc(f) == ' ', "char after the lone sign was pushed back");
        CHECK(next_int(f, &v) == 0 && v == 9,
              "non-number: returns 0, *out untouched");
        CHECK(fgetc(f) == 'z', "the offending char itself was pushed back");
        CHECK(next_int(f, &v) == 1 && v == 100,
              "recovers and reads the next number");
        CHECK(next_int(f, &v) == -1, "trailing whitespace then EOF gives -1");
        fclose(f);
    }
    {
        FILE *f = stream_of("12\n34");
        int v = 0;
        CHECK(next_int(f, &v) == 1 && v == 12, "number at start of stream");
        CHECK(next_int(f, &v) == 1 && v == 34,
              "newline is skipped like any whitespace");
        CHECK(next_int(f, &v) == -1, "number flush against EOF, then -1");
        fclose(f);
    }

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
