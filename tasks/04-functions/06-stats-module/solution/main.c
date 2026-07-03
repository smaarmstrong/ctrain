/* Driver — provided complete; you should not need to change it. */
#include <stdio.h>

#include "stats.h"

int main(void)
{
    long values[1000];
    size_t n = 0;
    long v;

    while (n < 1000 && scanf("%ld", &v) == 1)
        values[n++] = v;

    struct stats s = stats_of(values, n);
    if (s.count == 0) {
        printf("no data\n");
        return 1;
    }
    printf("count %zu\n", s.count);
    printf("min %ld\n", s.min);
    printf("max %ld\n", s.max);
    printf("sum %ld\n", s.sum);
    printf("mean %.2f\n", s.mean);
    return 0;
}
