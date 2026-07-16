#include <stdio.h>

static long fact(int n)
{
    if (n == 0)
        return 1;
    return n * fact(n - 1);
}

int main(void)
{
    printf("5! = %ld\n", fact(5));
    printf("7! = %ld\n", fact(7));
    return 0;
}
