/* Test harness — provides main() and calls the learner's functions. */
#include <stdbool.h>
#include <stdio.h>

int digit_value(char c);
int hex_value(char c);
char flip_case(char c);
bool parse_decimal(const char *s, long *out);

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

static bool parses_to(const char *s, long want)
{
    long v = 0;
    return parse_decimal(s, &v) && v == want;
}

static bool rejects(const char *s)
{
    long sentinel = 123456789L;
    if (parse_decimal(s, &sentinel))
        return false;
    return sentinel == 123456789L; /* *out untouched on failure */
}

int main(void)
{
    /* digit_value */
    CHECK(digit_value('0') == 0, "digit_value('0') is 0");
    CHECK(digit_value('9') == 9, "digit_value('9') is 9");
    CHECK(digit_value('5') == 5, "digit_value('5') is 5");
    CHECK(digit_value('a') == -1, "digit_value('a') is -1");
    CHECK(digit_value(' ') == -1, "digit_value(' ') is -1");
    CHECK(digit_value('/') == -1, "digit_value('/') is -1 (just below '0')");
    CHECK(digit_value(':') == -1, "digit_value(':') is -1 (just above '9')");

    /* hex_value */
    CHECK(hex_value('0') == 0 && hex_value('9') == 9, "hex 0-9");
    CHECK(hex_value('a') == 10 && hex_value('f') == 15, "hex a-f");
    CHECK(hex_value('A') == 10 && hex_value('F') == 15, "hex A-F");
    CHECK(hex_value('g') == -1, "hex_value('g') is -1");
    CHECK(hex_value('G') == -1, "hex_value('G') is -1");
    CHECK(hex_value('`') == -1, "hex_value('`') is -1 (just below 'a')");

    /* flip_case */
    CHECK(flip_case('a') == 'A', "flip_case('a') is 'A'");
    CHECK(flip_case('z') == 'Z', "flip_case('z') is 'Z'");
    CHECK(flip_case('A') == 'a', "flip_case('A') is 'a'");
    CHECK(flip_case('Q') == 'q', "flip_case('Q') is 'q'");
    CHECK(flip_case('3') == '3', "digits pass through");
    CHECK(flip_case('_') == '_', "punctuation passes through");
    CHECK(flip_case('[') == '[', "'[' (just above 'Z') passes through");
    CHECK(flip_case('{') == '{', "'{' (just above 'z') passes through");

    /* parse_decimal — successes */
    CHECK(parses_to("0", 0), "\"0\" parses to 0");
    CHECK(parses_to("42", 42), "\"42\" parses to 42");
    CHECK(parses_to("042", 42), "\"042\" parses to 42");
    CHECK(parses_to("-7", -7), "\"-7\" parses to -7");
    CHECK(parses_to("123456789", 123456789L), "a long-ish number parses");
    CHECK(parses_to("-9876543", -9876543L), "a negative number parses");

    /* parse_decimal — rejections, *out untouched */
    CHECK(rejects(NULL), "NULL is rejected");
    CHECK(rejects(""), "\"\" is rejected");
    CHECK(rejects("-"), "\"-\" is rejected");
    CHECK(rejects("--5"), "\"--5\" is rejected");
    CHECK(rejects(" 7"), "leading space is rejected");
    CHECK(rejects("7 "), "trailing space is rejected");
    CHECK(rejects("1x"), "trailing junk is rejected");
    CHECK(rejects("x1"), "leading junk is rejected");
    CHECK(rejects("4-2"), "embedded '-' is rejected");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
