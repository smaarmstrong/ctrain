#include <stdio.h>

int main(void)
{
    int score;
    while (scanf("%d", &score) == 1) {
        /* TODO: range-check, band with an if/else chain,
         * pass/fail with the ternary operator. */
        printf("%d: F (fail)\n", score);
    }
    return 0;
}
