#include <stdio.h>

/* TODO: turn the bounds into TABLE_LO / TABLE_HI macros with
 * defaults of 1 and 10 that a -D flag can override. */

int main(void)
{
    for (int n = 1; n <= 10; n++)
        printf("%d %d\n", n, n * n);
    return 0;
}
