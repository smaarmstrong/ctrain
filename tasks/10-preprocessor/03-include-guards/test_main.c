/* Test harness — includes both headers repeatedly and in both orders.
 * Without correct, per-header guards this file does not compile (or,
 * with a copy-pasted guard name, loses one header's definitions). */
#include "temperature.h"
#include "alarm.h"
#include "alarm.h"
#include "temperature.h"

#include <stdio.h>

static int failures;

#define CHECK(cond, msg)                                        \
    do {                                                        \
        if (cond) {                                             \
            printf("  ok    %s\n", msg);                        \
        } else {                                                \
            printf("  FAIL  %s\n", msg);                        \
            failures++;                                         \
        }                                                       \
    } while (0)

int main(void)
{
    struct temp_reading cold = {-1250};
    struct temp_reading warm = {3010};
    struct temp_reading hot = {4000};

    CHECK(temp_whole(cold) == -12, "temp_whole(-12.50C) == -12");
    CHECK(temp_whole(warm) == 30, "temp_whole(30.10C) == 30");
    CHECK(temp_alarm(cold) == ALARM_NONE, "-12.50C raises no alarm");
    CHECK(temp_alarm(warm) == ALARM_WARN, "30.10C raises a warning");
    CHECK(temp_alarm(hot) == ALARM_CRIT, "40.00C is critical");
    CHECK(ALARM_NONE == 0 && ALARM_WARN == 1 && ALARM_CRIT == 2,
          "alarm_level enumerators keep their values");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
