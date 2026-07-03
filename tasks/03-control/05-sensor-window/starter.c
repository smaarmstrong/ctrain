#include <stdio.h>

int main(void)
{
    int v, valid = 0, skipped = 0, min = 0, max = 0;

    while (scanf("%d", &v) == 1) {
        /* TODO: stop at the 9999 sentinel (break), count and skip
         * glitches outside -50..150 (continue), and only then treat
         * v as a valid reading. */
        if (valid == 0 || v < min)
            min = v;
        if (valid == 0 || v > max)
            max = v;
        valid++;
    }

    printf("valid: %d\n", valid);
    printf("skipped: %d\n", skipped);
    if (valid > 0) {
        printf("min: %d\n", min);
        printf("max: %d\n", max);
    }
    return 0;
}
