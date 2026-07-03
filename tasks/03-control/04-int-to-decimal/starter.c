void int_to_decimal(int n, char out[])
{
    /* TODO: emit digits with a do-while (lowest digit first, then
     * reverse — or however you like), handle the sign, and mind
     * INT_MIN: -n overflows when n is INT_MIN. */
    (void)n;
    out[0] = '0';
    out[1] = '\0';
}
