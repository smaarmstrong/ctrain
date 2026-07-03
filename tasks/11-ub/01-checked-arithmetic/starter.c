#include <stdbool.h>

/* TODO: these versions just do the operation and hope for the best.
 * They never report overflow, and when the result does not fit they
 * have already executed undefined behaviour. Replace each body with a
 * check that decides BEFORE operating. */

bool add_ok(int a, int b, int *result)
{
    *result = a + b;
    return true;
}

bool sub_ok(int a, int b, int *result)
{
    *result = a - b;
    return true;
}

bool mul_ok(int a, int b, int *result)
{
    *result = a * b;
    return true;
}
