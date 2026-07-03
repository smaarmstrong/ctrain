#include <stdio.h>

/* Keeps a running total of the integers on stdin... or should.
 * The compiler accepts this program without a murmur, yet the
 * final total is always zero. Why? */

int main(void)
{
    int total = 0;
    int count = 0;
    int value;

    while (scanf("%d", &value) == 1) {
        int total = 0; /* hmm. */
        total += value;
        count++;
        printf("add %d -> total %d\n", value, total);
    }

    printf("final total %d over %d values\n", total, count);
    return 0;
}
