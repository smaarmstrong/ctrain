/* temperature.h — a probe reading in hundredths of a degree Celsius. */
#ifndef CTRAIN_TEMPERATURE_H
#define CTRAIN_TEMPERATURE_H

struct temp_reading {
    int centi_c; /* 2150 means 21.50 degrees C */
};

static inline int temp_whole(struct temp_reading r)
{
    return r.centi_c / 100;
}

#endif /* CTRAIN_TEMPERATURE_H */
