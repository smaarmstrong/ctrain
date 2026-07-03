struct stats {
    int  count;
    long sum;
    int  min;
    int  max;
};

void stats_init(struct stats *s)
{
    /* TODO */
    (void)s;
}

void stats_add(struct stats *s, int value)
{
    /* TODO: use -> to update the fields. */
    (void)s;
    (void)value;
}

double stats_mean(const struct stats *s)
{
    /* TODO: 0.0 when empty. */
    (void)s;
    return 0.0;
}

void stats_merge(struct stats *dst, const struct stats *src)
{
    /* TODO: fold src's recorded values into dst. */
    (void)dst;
    (void)src;
}
