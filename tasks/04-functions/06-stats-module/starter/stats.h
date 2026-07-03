/* TODO: protect this header with an include guard so that a file
 * may include it twice and still compile. */

#include <stddef.h>

struct stats {
    size_t count; /* how many values                   */
    long min;     /* smallest value; 0 when count == 0 */
    long max;     /* largest value; 0 when count == 0  */
    long sum;     /* total; always fits in a long      */
    double mean;  /* sum / count; 0.0 when count == 0  */
};

/* Summarise the first n longs at values. n may be 0, in which
 * case values may be NULL and every field comes back zero. */
struct stats stats_of(const long *values, size_t n);
