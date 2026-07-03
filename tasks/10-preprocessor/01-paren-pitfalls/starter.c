/* Four macros that look right and compile — but each one expands
 * incorrectly inside a larger expression. Fix them.
 * Do not define main: the grader #includes this file into its harness. */

#define SQ(x) x * x

#define MIN(a, b) a < b ? a : b

#define MAX(a, b) a > b ? a : b

#define CLAMP(x, lo, hi) x < lo ? lo : x > hi ? hi : x
