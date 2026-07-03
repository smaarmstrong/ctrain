#include <stdio.h>

int main(void)
{
    int n;
    while (scanf("%d", &n) == 1) {
        /* TODO: validate n, then loop the Collatz step until the value
         * reaches 1, counting steps. Keep the changing value in a
         * long long — it can outgrow int on the way down. */
        printf("%d: 0\n", n);
    }
    return 0;
}
