#include <stdio.h>

#include "stats.h"

static const int readings[] = {12, 997, -5, 300, 41, 88, 512, 525};
#define NREADINGS (sizeof readings / sizeof readings[0])

int main(void)
{
    printf("n=%zu sum=%ld max=%d mean=%.2f\n",
           NREADINGS,
           sum_i(readings, NREADINGS),
           max_i(readings, NREADINGS),
           mean_i(readings, NREADINGS));
    return 0;
}
