/* Hint: a 2-row table of month lengths, one row per leap-ness.
 * static const int month_len[2][13] = { ... };
 */

int day_of_year(int year, int month, int day)
{
    /* TODO: validate month/day, then sum the months before this one. */
    (void)year;
    (void)month;
    (void)day;
    return -1;
}

int month_day(int year, int yearday, int *pmonth, int *pday)
{
    /* TODO: validate yearday, then subtract month lengths until it fits. */
    (void)year;
    (void)yearday;
    (void)pmonth;
    (void)pday;
    return -1;
}
