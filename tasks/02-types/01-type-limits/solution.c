#include <float.h>
#include <limits.h>
#include <stdio.h>

int main(void)
{
    printf("CHAR_BIT %d\n", CHAR_BIT);
    printf("sizeof(int) %zu\n", sizeof(int));
    printf("sizeof(long) %zu\n", sizeof(long));
    printf("INT_MIN %d\n", INT_MIN);
    printf("INT_MAX %d\n", INT_MAX);
    printf("UINT_MAX %u\n", UINT_MAX);
    printf("LLONG_MAX %lld\n", LLONG_MAX);
    printf("FLT_DIG %d\n", FLT_DIG);
    printf("DBL_DIG %d\n", DBL_DIG);
    printf("FLT_EPSILON %g\n", (double)FLT_EPSILON);
    printf("DBL_EPSILON %g\n", DBL_EPSILON);
    printf("WRAP %u\n", UINT_MAX + 1u);
    return 0;
}
