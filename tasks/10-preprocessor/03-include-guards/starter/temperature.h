/* temperature.h — a probe reading in hundredths of a degree Celsius.
 * FRAGILE: including this file twice in one translation unit redefines
 * the struct and the function. Add a guard. */

struct temp_reading {
    int centi_c; /* 2150 means 21.50 degrees C */
};

static inline int temp_whole(struct temp_reading r)
{
    return r.centi_c / 100;
}
