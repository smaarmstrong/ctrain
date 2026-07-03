# digits and case, by arithmetic

A `char` is just a small integer, and on any character set C supports,
the digits `'0'`–`'9'` are contiguous — so `c - '0'` is the digit's
value. This exercise builds four utilities out of nothing but char
arithmetic and comparisons. Assume ASCII (the grader's platforms are
ASCII), where `'a'`–`'z'` and `'A'`–`'Z'` are contiguous too.

Implement these in `main.c`. Do **not** define `main` — the grader
links your file against a test harness that provides it.

```c
#include <stdbool.h>

/* Value of a decimal digit character: '0'..'9' -> 0..9.
 * Return -1 for anything else. */
int digit_value(char c);

/* Value of a hexadecimal digit: '0'..'9' -> 0..9, 'a'..'f' and
 * 'A'..'F' -> 10..15. Return -1 for anything else. */
int hex_value(char c);

/* Flip the case of a letter: 'a' -> 'A', 'Q' -> 'q'. Any character
 * that is not an ASCII letter comes back unchanged. */
char flip_case(char c);

/* Parse s as a decimal integer: an optional single leading '-',
 * then one or more digits, nothing else. On success store the value
 * through out and return true. Return false (leaving *out alone) if
 * s is NULL, empty, is just "-", or contains any other character —
 * including leading spaces or trailing junk. Inputs that parse are
 * guaranteed to fit in a long. */
bool parse_decimal(const char *s, long *out);
```

Examples: `parse_decimal("042", &v)` succeeds with `v == 42`;
`parse_decimal("-7", &v)` succeeds with `v == -7`; `""`, `"-"`,
`" 7"`, `"7 "`, `"1x"` and `"--5"` all fail.

The harness rebuilds everything with AddressSanitizer and UBSan and
runs it again — reading past a string's terminator fails the task.

Grade with `ctrain check digits-and-case`.
