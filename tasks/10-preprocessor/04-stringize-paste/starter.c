#include <stdio.h>

/* Naive first attempts. Each compiles — and each misbehaves. */

/* stringizes the argument as written, never expanding it */
#define STR(x) #x

/* pastes before expanding, so JOIN(width_v, GEN) makes width_vGEN */
#define JOIN(a, b) a##b

/* prints the value but loses the expression text */
#define SHOW(expr) printf("%d\n", (expr))

/* no prefix, no newline */
#define LOG(...) printf(__VA_ARGS__)
