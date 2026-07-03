/* Test harness — provides main(); mode is selected by argv[1].
 *   clamp      normal clamped() behaviour (valid preconditions only)
 *   clamp-bad  calls clamped() with lo > hi and expects to survive —
 *              only run this mode on a -DNDEBUG build if you want exit 0
 *   port       parse_port() validation (identical in both builds)
 */
#include <stdio.h>
#include <string.h>

double clamped(double x, double lo, double hi);
int parse_port(const char *s);

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

static int finish(void)
{
    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s clamp|clamp-bad|port\n", argv[0]);
        return 2;
    }

    if (strcmp(argv[1], "clamp") == 0) {
        CHECK(clamped(3.0, 0.0, 10.0) == 3.0, "in range: unchanged");
        CHECK(clamped(-1.0, 0.0, 10.0) == 0.0, "below: clamps to lo");
        CHECK(clamped(11.0, 0.0, 10.0) == 10.0, "above: clamps to hi");
        CHECK(clamped(5.0, 5.0, 5.0) == 5.0, "lo == hi is a valid interval");
        CHECK(clamped(0.0, -2.5, -1.5) == -1.5, "negative interval");
        return finish();
    }

    if (strcmp(argv[1], "clamp-bad") == 0) {
        /* lo > hi violates the documented precondition. With asserts
         * enabled this call never returns; under NDEBUG it must. */
        double r = clamped(1.0, 5.0, 2.0);
        (void)r; /* whatever came back is fine — we only had to survive */
        printf("survived the precondition violation\n");
        return 0;
    }

    if (strcmp(argv[1], "port") == 0) {
        CHECK(parse_port("80") == 80, "\"80\" -> 80");
        CHECK(parse_port("1") == 1, "\"1\" -> 1");
        CHECK(parse_port("65535") == 65535, "\"65535\" -> 65535");
        CHECK(parse_port("0080") == 80, "leading zeros: \"0080\" -> 80");
        CHECK(parse_port("65536") == -1, "\"65536\" -> -1");
        CHECK(parse_port("0") == -1, "\"0\" -> -1");
        CHECK(parse_port("") == -1, "\"\" -> -1");
        CHECK(parse_port(NULL) == -1, "NULL -> -1");
        CHECK(parse_port("8a0") == -1, "\"8a0\" -> -1");
        CHECK(parse_port("-5") == -1, "\"-5\" -> -1");
        CHECK(parse_port(" 80") == -1, "\" 80\" -> -1");
        CHECK(parse_port("99999999999999999999") == -1,
              "20-digit string -> -1 (no accumulator overflow)");
        return finish();
    }

    fprintf(stderr, "unknown mode: %s\n", argv[1]);
    return 2;
}
