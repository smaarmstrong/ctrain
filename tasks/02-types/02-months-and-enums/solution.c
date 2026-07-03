#include <stdbool.h>

enum month {
    JAN = 1, FEB, MAR, APR, MAY, JUN,
    JUL, AUG, SEP, OCT, NOV, DEC
};

const char *month_name(int m)
{
    static const char *const names[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    if (m < JAN || m > DEC)
        return "?";
    return names[m - 1];
}

int days_in_month(int m, bool leap)
{
    static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (m < JAN || m > DEC)
        return 0;
    if (m == FEB && leap)
        return 29;
    return days[m - 1];
}

int day_of_year(int m, int d, bool leap)
{
    if (m < JAN || m > DEC || d < 1 || d > days_in_month(m, leap))
        return -1;
    int total = d;
    for (int i = JAN; i < m; i++)
        total += days_in_month(i, leap);
    return total;
}
