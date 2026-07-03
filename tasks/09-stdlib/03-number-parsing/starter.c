#include <errno.h>
#include <stdlib.h>

int parse_long(const char *s, long *out)
{
    /* TODO: errno = 0; strtol with an endptr; then check, in order:
     *   no conversion (endptr == s)      -> -1
     *   non-whitespace after the number  -> -1
     *   errno == ERANGE                  -> -2
     * only then store and return 0. */
    *out = atoi(s); /* atoi can't report any of that — replace it */
    return 0;
}

int parse_double(const char *s, double *out)
{
    /* TODO: same shape with strtod. */
    *out = atof(s);
    return 0;
}
