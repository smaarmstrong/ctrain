#ifndef STATS_H
#define STATS_H

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

#endif /* STATS_H */
