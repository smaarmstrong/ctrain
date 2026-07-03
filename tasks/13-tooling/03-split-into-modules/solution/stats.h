#ifndef STATS_H
#define STATS_H

#include <stddef.h>

/* Sum of the first n ints; 0 when n is 0. */
long sum_i(const int *a, size_t n);

/* Largest of the first n ints; the caller guarantees n >= 1. */
int max_i(const int *a, size_t n);

/* Arithmetic mean of the first n ints; the caller guarantees n >= 1. */
double mean_i(const int *a, size_t n);

#endif
