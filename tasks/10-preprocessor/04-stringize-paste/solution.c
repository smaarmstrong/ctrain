#include <stdio.h>

/* # and ## suppress expansion of their operands, so route the argument
 * through a helper: by the time the helper's body applies # or ##, the
 * argument has already been expanded. */

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define JOIN_HELPER(a, b) a##b
#define JOIN(a, b) JOIN_HELPER(a, b)

/* Here the UNexpanded spelling is wanted, so plain # is exactly right. */
#define SHOW(expr) printf("%s = %d\n", #expr, (expr))

/* __VA_ARGS__ carries everything, format string included. */
#define LOG(...) (printf("[log] "), printf(__VA_ARGS__), printf("\n"))
