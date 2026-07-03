/* Test harness — provides main() and calls the learner's functions. */
#include "palette.h"

#include <stdio.h>
#include <string.h>

_Static_assert(PAL_COUNT == 6, "the palette must have exactly six colors");

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

static void check_entry(int id, unsigned r, unsigned g, unsigned b,
                        const char *name, int luma)
{
    struct color c = palette_get(id);
    char msg[128];

    snprintf(msg, sizeof msg, "palette_get(%d) is (%u, %u, %u) \"%s\"",
             id, r, g, b, name);
    CHECK(c.r == r && c.g == g && c.b == b &&
          c.name != NULL && strcmp(c.name, name) == 0, msg);

    snprintf(msg, sizeof msg, "color_luma of \"%s\" is %d", name, luma);
    CHECK(color_luma(c) == luma, msg);
}

int main(void)
{
    check_entry(PAL_BLACK, 0, 0, 0, "black", 0);
    check_entry(PAL_RED, 255, 0, 0, "red", 76);
    check_entry(PAL_GREEN, 0, 255, 0, "green", 149);
    check_entry(PAL_BLUE, 0, 0, 255, "blue", 29);
    check_entry(PAL_YELLOW, 255, 255, 0, "yellow", 225);
    check_entry(PAL_WHITE, 255, 255, 255, "white", 255);

    check_entry(-1, 0, 0, 0, "unknown", 0);
    check_entry(PAL_COUNT, 0, 0, 0, "unknown", 0);
    check_entry(999, 0, 0, 0, "unknown", 0);

    struct color custom = {10, 20, 30, "custom"};
    CHECK(color_luma(custom) == 18,
          "color_luma works on a non-palette color (10,20,30 -> 18)");

    CHECK(palette_brightest() == PAL_WHITE, "palette_brightest() is white");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
