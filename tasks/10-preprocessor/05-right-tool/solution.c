/* Preprocessor visibility (#if) is required -> only a macro can do it. */
#define BUF_SIZE 32

/* Must work for any element type AND be an integer constant expression
 * usable at file scope -> a sizeof-based macro; a function cannot. */
#define ARRAY_LEN(a) (sizeof(a) / sizeof((a)[0]))

/* Each argument evaluated exactly once -> a real function; C's call
 * semantics guarantee it, no amount of macro parenthesising can. */
int clampi(int x, int lo, int hi)
{
    if (x < lo)
        return lo;
    if (x > hi)
        return hi;
    return x;
}
