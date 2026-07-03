#include <stdbool.h>
#include <limits.h>

bool add_ok(int a, int b, int *result)
{
    if (b > 0 ? a > INT_MAX - b : a < INT_MIN - b)
        return false;
    *result = a + b;
    return true;
}

bool sub_ok(int a, int b, int *result)
{
    if (b > 0 ? a < INT_MIN + b : a > INT_MAX + b)
        return false;
    *result = a - b;
    return true;
}

bool mul_ok(int a, int b, int *result)
{
    long long p = (long long)a * (long long)b;
    if (p > INT_MAX || p < INT_MIN)
        return false;
    *result = (int)p;
    return true;
}
