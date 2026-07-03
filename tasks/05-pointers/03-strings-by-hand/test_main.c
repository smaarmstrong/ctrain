/* Test harness — provides main() and calls the learner's functions. */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t str_length(const char *s);
char *str_copy(char *dst, const char *src);
int str_compare(const char *a, const char *b);

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

/* Heap copy sized exactly strlen+1: under ASan any read past the '\0'
 * lands in a redzone and aborts the run. */
static char *exact(const char *s)
{
    size_t n = strlen(s) + 1;
    char *p = malloc(n);
    if (!p)
        exit(2);
    memcpy(p, s, n);
    return p;
}

static int sign(int v) { return (v > 0) - (v < 0); }

int main(void)
{
    /* str_length */
    char *hello = exact("hello");
    char *empty = exact("");
    CHECK(str_length(hello) == 5, "str_length(\"hello\") == 5");
    CHECK(str_length(empty) == 0, "str_length(\"\") == 0");
    char embedded[5] = {'a', 'b', '\0', 'z', '\0'};
    CHECK(str_length(embedded) == 2, "length stops at the first '\\0'");

    /* str_copy */
    char buf[32];
    memset(buf, 'X', sizeof buf);
    char *ret = str_copy(buf, hello);
    CHECK(ret == buf, "str_copy returns dst");
    CHECK(memcmp(buf, "hello", 6) == 0, "str_copy copies the bytes and the '\\0'");
    CHECK(buf[6] == 'X', "str_copy writes nothing past the terminator");
    str_copy(buf, empty);
    CHECK(buf[0] == '\0', "copying \"\" yields \"\"");

    /* str_copy into an exactly-sized heap buffer: one byte too many trips ASan */
    char *tight = malloc(6);
    if (!tight)
        return 2;
    str_copy(tight, hello);
    CHECK(memcmp(tight, "hello", 6) == 0, "copy into an exactly-sized buffer");
    free(tight);

    /* str_compare — sign only */
    char *apple = exact("apple");
    char *app = exact("app");
    char *appl2 = exact("apple");
    CHECK(sign(str_compare(apple, appl2)) == 0, "equal strings compare 0");
    CHECK(sign(str_compare(empty, empty)) == 0, "\"\" equals \"\"");
    CHECK(sign(str_compare(app, apple)) < 0, "prefix sorts before the longer string");
    CHECK(sign(str_compare(apple, app)) > 0, "...and the reverse is positive");
    char *aaa = exact("aaa");
    char *aab = exact("aab");
    CHECK(sign(str_compare(aaa, aab)) < 0, "differ in the last byte");
    CHECK(sign(str_compare(aab, aaa)) > 0, "...and the reverse");
    char *lo = exact("\x7f");
    char *hi = exact("\x80");
    CHECK(sign(str_compare(lo, hi)) < 0, "comparison is unsigned (0x7f < 0x80)");
    CHECK(sign(str_compare(empty, aaa)) < 0, "\"\" sorts before everything else");

    free(hello); free(empty);
    free(apple); free(app); free(appl2);
    free(aaa); free(aab); free(lo); free(hi);

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
