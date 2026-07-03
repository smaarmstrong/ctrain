#include <stdio.h>

int main(void)
{
    int n;
    while (scanf("%d", &n) == 1) {
        if (n < 1) {
            printf("%d: invalid\n", n);
            continue;
        }
        int steps = 0;
        for (long long v = n; v != 1; steps++)
            v = (v % 2 == 0) ? v / 2 : 3 * v + 1;
        printf("%d: %d\n", n, steps);
    }
    return 0;
}
