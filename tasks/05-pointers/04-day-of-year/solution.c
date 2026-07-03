static const int month_len[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
};

static int is_leap(int y)
{
    return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
}

int day_of_year(int year, int month, int day)
{
    int leap = is_leap(year);
    if (month < 1 || month > 12)
        return -1;
    if (day < 1 || day > month_len[leap][month])
        return -1;
    int total = day;
    for (int m = 1; m < month; m++)
        total += month_len[leap][m];
    return total;
}

int month_day(int year, int yearday, int *pmonth, int *pday)
{
    int leap = is_leap(year);
    if (yearday < 1 || yearday > 365 + leap)
        return -1;
    int m = 1;
    while (yearday > month_len[leap][m]) {
        yearday -= month_len[leap][m];
        m++;
    }
    *pmonth = m;
    *pday = yearday;
    return 0;
}
