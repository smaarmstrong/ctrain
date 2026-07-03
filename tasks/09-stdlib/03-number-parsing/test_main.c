/* Test harness — provides main() and calls the learner's functions. */
#include <errno.h>
#include <stdio.h>

int parse_long(const char *s, long *out);
int parse_double(const char *s, double *out);

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
    long v;
    double d;

    /* parse_long: successes */
    v = 0;
    CHECK(parse_long("42", &v) == 0 && v == 42, "\"42\" -> OK, 42");
    v = 0;
    CHECK(parse_long("  -7  ", &v) == 0 && v == -7,
          "surrounding whitespace is fine: \"  -7  \" -> OK, -7");
    v = 0;
    CHECK(parse_long("+5", &v) == 0 && v == 5, "\"+5\" -> OK, 5");

    /* parse_long: PARSE_BAD */
    v = 111;
    CHECK(parse_long("12abc", &v) == -1, "\"12abc\" -> PARSE_BAD");
    CHECK(v == 111, "*out untouched on failure");
    CHECK(parse_long("abc", &v) == -1, "\"abc\" -> PARSE_BAD");
    CHECK(parse_long("", &v) == -1, "\"\" -> PARSE_BAD");
    CHECK(parse_long("   ", &v) == -1, "whitespace only -> PARSE_BAD");
    CHECK(parse_long("1 2", &v) == -1,
          "second token after the number -> PARSE_BAD");

    /* parse_long: PARSE_RANGE */
    CHECK(parse_long("99999999999999999999", &v) == -2,
          "overflow -> PARSE_RANGE");
    CHECK(parse_long("-99999999999999999999", &v) == -2,
          "negative overflow -> PARSE_RANGE");
    CHECK(parse_long("99999999999999999999xyz", &v) == -1,
          "malformed beats out-of-range: -> PARSE_BAD");

    /* parse_long: must reset errno itself */
    errno = ERANGE;
    v = 0;
    CHECK(parse_long("42", &v) == 0 && v == 42,
          "stale ERANGE in errno does not fake a range error");

    /* parse_double: successes (exactly representable values) */
    d = 0;
    CHECK(parse_double("2.5e3", &d) == 0 && d == 2500.0, "\"2.5e3\" -> OK, 2500");
    d = 0;
    CHECK(parse_double(".5", &d) == 0 && d == 0.5, "\".5\" -> OK, 0.5");
    d = 0;
    CHECK(parse_double(" \t3.25 ", &d) == 0 && d == 3.25,
          "tabs and spaces around \"3.25\" are fine");
    d = 0;
    CHECK(parse_double("-4", &d) == 0 && d == -4.0, "\"-4\" -> OK, -4");

    /* parse_double: failures */
    d = 9.5;
    CHECK(parse_double("3.5.7", &d) == -1, "\"3.5.7\" -> PARSE_BAD");
    CHECK(d == 9.5, "*out untouched on failure (double)");
    CHECK(parse_double("e5", &d) == -1, "\"e5\" -> PARSE_BAD");
    CHECK(parse_double("1e999", &d) == -2, "\"1e999\" -> PARSE_RANGE");
    CHECK(parse_double("-1e999", &d) == -2, "\"-1e999\" -> PARSE_RANGE");

    errno = ERANGE;
    d = 0;
    CHECK(parse_double("1.5", &d) == 0 && d == 1.5,
          "stale ERANGE in errno does not fake a range error (double)");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
