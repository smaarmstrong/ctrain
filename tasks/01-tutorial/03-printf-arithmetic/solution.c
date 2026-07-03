#include <stdio.h>

int main(void)
{
    int a = 1234;
    int b = 56;

    printf("a=%d b=%d\n", a, b);
    printf("sum=%d diff=%d\n", a + b, a - b);
    printf("quot=%d rem=%d\n", a / b, a % b);
    printf("ratio=%.3f\n", (double)a / b);
    printf("pct=%.2f%%\n", 100.0 * b / a);
    printf("boxed=[%6d]\n", a);
    return 0;
}
