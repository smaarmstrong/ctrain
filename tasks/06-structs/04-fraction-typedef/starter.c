typedef struct {
    long num;
    long den;
} Frac;

Frac frac_make(long num, long den)
{
    /* TODO: reduce by the gcd; den must end up positive; zero is 0/1. */
    Frac f = {num, den};
    return f;
}

Frac frac_add(Frac a, Frac b)
{
    /* TODO */
    (void)b;
    return a;
}

Frac frac_mul(Frac a, Frac b)
{
    /* TODO */
    (void)b;
    return a;
}

int frac_cmp(Frac a, Frac b)
{
    /* TODO: -1, 0 or 1 by value. */
    (void)a;
    (void)b;
    return 0;
}
