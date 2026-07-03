typedef struct {
    long num;
    long den;
} Frac;

static long gcd(long a, long b)
{
    if (a < 0)
        a = -a;
    if (b < 0)
        b = -b;
    while (b != 0) {
        long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

Frac frac_make(long num, long den)
{
    if (num == 0) {
        Frac zero = {0, 1};
        return zero;
    }
    if (den < 0) {
        num = -num;
        den = -den;
    }
    long g = gcd(num, den);
    Frac f = {num / g, den / g};
    return f;
}

Frac frac_add(Frac a, Frac b)
{
    return frac_make(a.num * b.den + b.num * a.den, a.den * b.den);
}

Frac frac_mul(Frac a, Frac b)
{
    return frac_make(a.num * b.num, a.den * b.den);
}

int frac_cmp(Frac a, Frac b)
{
    long lhs = a.num * b.den;
    long rhs = b.num * a.den;
    if (lhs < rhs)
        return -1;
    if (lhs > rhs)
        return 1;
    return 0;
}
