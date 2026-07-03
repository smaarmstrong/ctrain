/* Reference solution. */

long ipow(int base, int exp)
{
    long result = 1;
    while (exp-- > 0)
        result *= base;
    return result;
}

int clampi(int x, int lo, int hi)
{
    if (x < lo)
        return lo;
    if (x > hi)
        return hi;
    return x;
}

int digit_count(long n)
{
    int count = 1;
    while (n / 10 != 0) {
        n /= 10;
        count++;
    }
    return count;
}
