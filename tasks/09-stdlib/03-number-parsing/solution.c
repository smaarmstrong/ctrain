#include <ctype.h>
#include <errno.h>
#include <stdlib.h>

static int tail_is_blank(const char *p)
{
    while (isspace((unsigned char)*p))
        p++;
    return *p == '\0';
}

int parse_long(const char *s, long *out)
{
    char *end;
    errno = 0;
    long v = strtol(s, &end, 10);
    if (end == s || !tail_is_blank(end))
        return -1;
    if (errno == ERANGE)
        return -2;
    *out = v;
    return 0;
}

int parse_double(const char *s, double *out)
{
    char *end;
    errno = 0;
    double v = strtod(s, &end);
    if (end == s || !tail_is_blank(end))
        return -1;
    if (errno == ERANGE)
        return -2;
    *out = v;
    return 0;
}
