#include <math.h>

const char *fp_class(double x)
{
    /* TODO: fpclassify(x) gives FP_NAN, FP_INFINITE, FP_ZERO,
     * FP_SUBNORMAL, FP_NORMAL; split infinity by sign. */
    (void)x;
    return "unknown";
}

long long ll_round(double x)
{
    /* TODO: one math.h call rounds halves away from zero. */
    return (long long)x; /* truncates — wrong for 2.5 and for negatives */
}

double round_places(double x, int places)
{
    /* TODO: scale by 10^places, round, scale back. */
    (void)places;
    return x;
}
