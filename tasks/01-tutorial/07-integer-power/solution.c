long int_pow(int base, int exp)
{
    long result = 1;

    for (int i = 0; i < exp; i++)
        result *= base;
    return result;
}
