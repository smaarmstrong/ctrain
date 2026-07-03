/* main.c — works today only because it includes alarm.h alone.
 * You should not need to touch this file. */
#include <stdio.h>

#include "alarm.h"

static const char *alarm_name(enum alarm_level a)
{
    switch (a) {
    case ALARM_WARN:
        return "warn";
    case ALARM_CRIT:
        return "crit";
    default:
        return "none";
    }
}

int main(void)
{
    struct temp_reading probes[] = {{2150}, {3500}, {4120}};
    for (size_t i = 0; i < sizeof probes / sizeof probes[0]; i++) {
        struct temp_reading r = probes[i];
        printf("%d.%02dC -> whole %d, alarm %s\n",
               r.centi_c / 100, r.centi_c % 100,
               temp_whole(r), alarm_name(temp_alarm(r)));
    }
    return 0;
}
