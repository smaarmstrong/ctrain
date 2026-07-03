/* Rule of thumb: parenthesise the whole body AND every use of a
 * parameter. Then the expansion binds the same way a call would. */

#define SQ(x) ((x) * (x))

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))
