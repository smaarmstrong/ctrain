#include <stdio.h>

int main(void)
{
    int total = 0;
    int count = 0;
    int value;

    while (scanf("%d", &value) == 1) {
        total += value;
        count++;
        printf("add %d -> total %d\n", value, total);
    }

    printf("final total %d over %d values\n", total, count);
    return 0;
}
