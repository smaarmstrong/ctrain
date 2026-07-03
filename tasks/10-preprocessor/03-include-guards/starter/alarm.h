/* alarm.h — classify a reading. Needs struct temp_reading, so it
 * includes temperature.h itself.
 * FRAGILE: double inclusion redefines the enum and the function. */

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
