#include <limits.h>
#include <stddef.h>

double mean_of(const int *a, size_t n)
{
    if (n == 0)
        return 0.0;
    long long sum = 0;
    for (size_t i = 0; i < n; i++)
        sum += a[i];
    return (double)sum / (double)n;
}

int percent_of(int part, int whole)
{
    return (int)((long long)part * 100 / whole);
}

unsigned char low_byte(long v)
{
    return (unsigned char)v;
}

int clamp_to_int(double d)
{
    if (d >= 2147483648.0) /* INT_MAX + 1, exact as a double */
        return INT_MAX;
    if (d <= -2147483649.0) /* INT_MIN - 1, exact as a double */
        return INT_MIN;
    return (int)d;
}
