#include <math.h>

const char *fp_class(double x)
{
    switch (fpclassify(x)) {
    case FP_NAN:
        return "nan";
    case FP_INFINITE:
        return x > 0 ? "+inf" : "-inf";
    case FP_ZERO:
        return "zero";
    case FP_SUBNORMAL:
        return "subnormal";
    default:
        return "normal";
    }
}

long long ll_round(double x)
{
    return llround(x);
}

double round_places(double x, int places)
{
    double scale = pow(10.0, places);
    return round(x * scale) / scale;
}
