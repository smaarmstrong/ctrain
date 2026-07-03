#include <stdio.h>

#ifndef TABLE_LO
#define TABLE_LO 1
#endif

#ifndef TABLE_HI
#define TABLE_HI 10
#endif

int main(void)
{
    for (int n = TABLE_LO; n <= TABLE_HI; n++)
        printf("%d %d\n", n, n * n);
    return 0;
}
