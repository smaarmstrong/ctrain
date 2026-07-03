/* Three utilities, each built with a tool that ALMOST works.
 * Everything compiles; the grader's behaviour checks say otherwise. */

/* An enum constant is a fine array size... but #if cannot see it:
 * in `#if BUF_SIZE == 32` an unknown identifier just becomes 0. */
enum { BUF_SIZE = 32 };

/* Compiles, and even works for int arrays — but the divisor is wrong
 * for every other element type. */
#define ARRAY_LEN(a) (sizeof(a) / (sizeof(int)))

/* Parenthesised perfectly, yet it still evaluates x up to three
 * times — visible as soon as an argument has side effects. */
#define clampi(x, lo, hi) \
    ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))
