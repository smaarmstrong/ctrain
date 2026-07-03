struct stats {
    int  count;
    long sum;
    int  min;
    int  max;
};

void stats_init(struct stats *s)
{
    s->count = 0;
    s->sum = 0;
    s->min = 0;
    s->max = 0;
}

void stats_add(struct stats *s, int value)
{
    if (s->count == 0 || value < s->min)
        s->min = value;
    if (s->count == 0 || value > s->max)
        s->max = value;
    s->count++;
    s->sum += value;
}

double stats_mean(const struct stats *s)
{
    if (s->count == 0)
        return 0.0;
    return (double)s->sum / s->count;
}

void stats_merge(struct stats *dst, const struct stats *src)
{
    if (src->count == 0)
        return;
    if (dst->count == 0 || src->min < dst->min)
        dst->min = src->min;
    if (dst->count == 0 || src->max > dst->max)
        dst->max = src->max;
    dst->count += src->count;
    dst->sum += src->sum;
}
