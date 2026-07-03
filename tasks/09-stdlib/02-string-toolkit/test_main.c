/* Test harness — provides main() and calls the learner's functions. */
#include <stddef.h>
#include <stdio.h>
#include <string.h>

size_t count_occurrences(const char *hay, const char *needle);
size_t split_fields(char *line, char *fields[], size_t max);
void erase_range(char *s, size_t pos, size_t count);

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
    /* count_occurrences */
    CHECK(count_occurrences("banana", "na") == 2, "\"na\" in \"banana\" -> 2");
    CHECK(count_occurrences("aaaa", "aa") == 2,
          "non-overlapping: \"aa\" in \"aaaa\" -> 2");
    CHECK(count_occurrences("abcabcabc", "abc") == 3, "back-to-back matches");
    CHECK(count_occurrences("hay", "needle") == 0, "no match -> 0");
    CHECK(count_occurrences("", "x") == 0, "empty haystack -> 0");
    CHECK(count_occurrences("abc", "") == 0, "empty needle -> 0");
    CHECK(count_occurrences("xyxy", "xyxy") == 1, "needle == haystack -> 1");

    /* split_fields */
    {
        char buf[] = "one,two,three";
        char *f[8] = {0};
        size_t n = split_fields(buf, f, 8);
        CHECK(n == 3, "\"one,two,three\" -> 3 fields");
        CHECK(n == 3 && strcmp(f[0], "one") == 0 && strcmp(f[1], "two") == 0 &&
                  strcmp(f[2], "three") == 0,
              "fields are \"one\" \"two\" \"three\"");
    }
    {
        char buf[] = "a,,b";
        char *f[8] = {0};
        size_t n = split_fields(buf, f, 8);
        CHECK(n == 2 && strcmp(f[0], "a") == 0 && strcmp(f[1], "b") == 0,
              "empty fields are skipped: \"a,,b\" -> \"a\" \"b\"");
    }
    {
        char buf[] = ",,,";
        char *f[8] = {0};
        CHECK(split_fields(buf, f, 8) == 0, "\",,,\" -> 0 fields");
    }
    {
        char buf[] = "";
        char *f[8] = {0};
        CHECK(split_fields(buf, f, 8) == 0, "empty line -> 0 fields");
    }
    {
        char buf[] = ",lead,trail,";
        char *f[8] = {0};
        size_t n = split_fields(buf, f, 8);
        CHECK(n == 2 && strcmp(f[0], "lead") == 0 && strcmp(f[1], "trail") == 0,
              "commas at either end produce no fields");
    }
    {
        char buf[] = "a,b,c,d";
        char *f[2] = {0};
        size_t n = split_fields(buf, f, 2);
        CHECK(n == 2 && strcmp(f[0], "a") == 0 && strcmp(f[1], "b") == 0,
              "max caps the number of stored fields");
    }
    {
        char buf[] = "a,b";
        CHECK(split_fields(buf, NULL, 0) == 0, "max == 0 stores nothing");
    }

    /* erase_range */
    {
        char s[] = "hello world";
        erase_range(s, 5, 1);
        CHECK(strcmp(s, "helloworld") == 0, "erase 1 byte from the middle");
    }
    {
        char s[] = "abcdefgh";
        erase_range(s, 0, 3);
        CHECK(strcmp(s, "defgh") == 0, "erase a prefix");
    }
    {
        char s[] = "abcdefgh";
        erase_range(s, 5, 3);
        CHECK(strcmp(s, "abcde") == 0, "erase a suffix exactly to the end");
    }
    {
        char s[] = "abc";
        erase_range(s, 1, 99);
        CHECK(strcmp(s, "a") == 0, "count past the end is clamped");
    }
    {
        char s[] = "abc";
        erase_range(s, 3, 5);
        CHECK(strcmp(s, "abc") == 0, "pos == strlen leaves s unchanged");
    }
    {
        char s[] = "abc";
        erase_range(s, 1, 0);
        CHECK(strcmp(s, "abc") == 0, "count == 0 leaves s unchanged");
    }
    {
        /* long overlapping move — an overlapping memcpy shows up here */
        char s[64];
        memset(s, 'x', 60);
        s[60] = '\0';
        memcpy(s, "HEAD", 4);
        memcpy(s + 56, "TAIL", 4);
        erase_range(s, 4, 8);
        CHECK(strlen(s) == 52 && memcmp(s, "HEAD", 4) == 0 &&
                  memcmp(s + 48, "TAIL", 4) == 0,
              "large overlapping erase keeps both ends intact");
    }

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
