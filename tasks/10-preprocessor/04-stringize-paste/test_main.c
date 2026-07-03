/* Test harness — includes the learner's macros and prints through them;
 * the grader diffs stdout against expected.out. */
#include <stdio.h>

#include "main.c"

#define ANSWER 42
#define GEN 2

static int width_v2 = 640;
static int width_vGEN = -1; /* a paste-before-expand JOIN lands here */

int main(void)
{
    /* STR: stringize the EXPANDED argument */
    printf("%s\n", STR(ANSWER));
    printf("%s\n", STR(GEN + GEN));

    /* JOIN: paste AFTER expanding both sides */
    printf("%d\n", JOIN(width_v, GEN));
    (void)width_v2;
    (void)width_vGEN;

    /* SHOW: the literal expression text, then its value */
    int apples = 3, pears = 4;
    SHOW(apples);
    SHOW(apples + pears);
    SHOW(10 * apples - pears);

    /* LOG: [log] prefix, printf semantics, one trailing newline */
    LOG("startup");
    LOG("loaded %d items from %s", 7, "cache");
    LOG("%d%%", 99);

    return 0;
}
