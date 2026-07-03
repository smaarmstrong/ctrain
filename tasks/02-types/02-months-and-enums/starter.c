#include <stdbool.h>

/* TODO: declare an enum for the months, JAN = 1 .. DEC = 12. */

const char *month_name(int m)
{
    /* TODO: "January".."December"; "?" outside 1..12. */
    (void)m;
    return "?";
}

int days_in_month(int m, bool leap)
{
    /* TODO: 28/29 for February, 30 or 31 elsewhere; 0 outside 1..12. */
    (void)m;
    (void)leap;
    return 0;
}

int day_of_year(int m, int d, bool leap)
{
    /* TODO: 1-based ordinal within the year; -1 for invalid dates. */
    (void)m;
    (void)d;
    (void)leap;
    return -1;
}
