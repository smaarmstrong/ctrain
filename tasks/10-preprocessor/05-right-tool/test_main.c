/* Test harness — the checks are chosen so that only the right tool for
 * each job can pass them. */
#include <stdio.h>

#include "main.c"

/* 1. BUF_SIZE must be visible to the preprocessor with value 32.
 *    (If it is not a macro, defined() is false and this becomes 0.) */
#if defined(BUF_SIZE) && BUF_SIZE == 32
#define BUF_SIZE_SEEN_BY_PP 1
#else
#define BUF_SIZE_SEEN_BY_PP 0
#endif

/* ...and usable as a file-scope array size. */
static unsigned char scratch[BUF_SIZE];

/* 2. ARRAY_LEN must be an integer constant expression: file scope. */
static const double samples[] = {1.5, 2.5, 3.5, 4.5, 5.5};
static short mirror[ARRAY_LEN(samples)];

static int calls;
static int bump(void)
{
    calls++;
    return 7;
}

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
    /* BUF_SIZE */
    CHECK(BUF_SIZE_SEEN_BY_PP, "#if sees BUF_SIZE == 32");
    CHECK(sizeof scratch == 32, "scratch[BUF_SIZE] holds 32 bytes");

    /* ARRAY_LEN across element types */
    CHECK(ARRAY_LEN(samples) == 5, "ARRAY_LEN of 5 doubles is 5");
    CHECK(sizeof mirror / sizeof mirror[0] == 5,
          "mirror[ARRAY_LEN(samples)] has 5 elements");
    char msg[] = "hello";
    CHECK(ARRAY_LEN(msg) == 6 && msg[0] == 'h',
          "ARRAY_LEN counts the NUL of \"hello\"");
    struct pair { int a, b; };
    struct pair ps[3] = {{1, 2}, {3, 4}, {5, 6}};
    CHECK(ARRAY_LEN(ps) == 3 && ps[2].b == 6,
          "ARRAY_LEN of 3 structs is 3");

    /* clampi: values first... */
    CHECK(clampi(5, 0, 10) == 5, "clampi leaves in-range values alone");
    CHECK(clampi(-5, 0, 10) == 0, "clampi raises low values to lo");
    CHECK(clampi(50, 0, 10) == 10, "clampi lowers high values to hi");

    /* ...then single evaluation of every argument. */
    calls = 0;
    int r = clampi(bump(), 0, 10); /* bump() returns 7 */
    CHECK(r == 7 && calls == 1,
          "clampi evaluates an in-range x exactly once");
    calls = 0;
    r = clampi(20, bump(), bump() + 3); /* lo = 7, hi = 10 */
    CHECK(r == 10 && calls == 2,
          "clampi evaluates lo and hi exactly once each");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
