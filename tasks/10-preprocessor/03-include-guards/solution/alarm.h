/* alarm.h — classify a reading. Self-sufficient: pulls in
 * temperature.h itself; the guards make the nesting harmless. */
#ifndef CTRAIN_ALARM_H
#define CTRAIN_ALARM_H

#include "temperature.h"

enum alarm_level {
    ALARM_NONE,
    ALARM_WARN,
    ALARM_CRIT
};

static inline enum alarm_level temp_alarm(struct temp_reading r)
{
    if (r.centi_c >= 4000)
        return ALARM_CRIT;
    if (r.centi_c >= 3000)
        return ALARM_WARN;
    return ALARM_NONE;
}

#endif /* CTRAIN_ALARM_H */
