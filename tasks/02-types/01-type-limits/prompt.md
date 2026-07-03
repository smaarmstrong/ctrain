# a tour of the numeric limits

Every C implementation documents its numeric ranges in two headers:
`<limits.h>` for the integer types and `<float.h>` for the floating types.
Write a program that reports a selection of them, one per line, in the
exact format `NAME value` (one space between the two fields).

Print these twelve lines, in this order, taking every value from the
standard macros or from `sizeof` — do not type the numbers in yourself:

1. `CHAR_BIT` — bits in a `char`
2. `sizeof(int)` — print the name literally as `sizeof(int)`, the value
   with the `size_t` conversion specifier
3. `sizeof(long)` — same idea
4. `INT_MIN`
5. `INT_MAX`
6. `UINT_MAX` — mind the specifier: this one does not fit in an `int`
7. `LLONG_MAX` — a `long long`, so it needs the matching length modifier
8. `FLT_DIG` — decimal digits a `float` can round-trip
9. `DBL_DIG` — the same for `double`
10. `FLT_EPSILON` — printed with `%g`
11. `DBL_EPSILON` — printed with `%g`
12. `WRAP` — the value of the expression `UINT_MAX + 1u` (unsigned
    arithmetic wraps around; this line demonstrates it)

The first three lines on a typical 64-bit Linux system look like:

```
CHAR_BIT 8
sizeof(int) 4
sizeof(long) 8
```

Print nothing else — no headers, no blank lines. Every line ends with a
single newline. Exit with status 0.

A mismatched conversion specifier (for example printing an `unsigned`
with `%d`) is a compile error here: the grader builds with
`-Wall -Wextra -Werror`.

Your code goes in `main.c`. Grade with `ctrain check type-limits`.
