void int_to_decimal(int n, char out[])
{
    long long v = n; /* wider type: -INT_MIN fits in long long */
    int i = 0;

    if (v < 0) {
        out[i++] = '-';
        v = -v;
    }

    /* emit digits lowest-first into a scratch area, at least once */
    char digits[11];
    int d = 0;
    do {
        digits[d++] = (char)('0' + v % 10);
        v /= 10;
    } while (v > 0);

    while (d > 0)
        out[i++] = digits[--d];
    out[i] = '\0';
}
