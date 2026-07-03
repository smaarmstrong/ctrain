# strings by hand

A C string is just a pointer to its first character; the string ends at
the first `'\0'` byte. Implement the three classic primitives yourself
by walking the bytes with a pointer (or an index — your choice).

Implement these functions in `main.c`. Do **not** define `main` — the
grader links your file against a test harness that provides it.

```c
#include <stddef.h>

/* Number of characters before the terminating '\0'.
 * str_length("") is 0. */
size_t str_length(const char *s);

/* Copy src (including its '\0') into dst and return dst.
 * The caller guarantees dst is large enough. dst and src never
 * overlap. */
char *str_copy(char *dst, const char *src);

/* Compare a and b byte by byte as unsigned chars.
 * Return a negative value if a sorts before b, 0 if they are equal,
 * a positive value if a sorts after b. Only the SIGN is checked, not
 * the magnitude. */
int str_compare(const char *a, const char *b);
```

Rules:

- **You may not use `<string.h>`.** The grader rejects your file if any
  of these identifiers appear anywhere in it (even in a comment):
  `strlen`, `strcpy`, `strncpy`, `strcmp`, `strncmp`, `strcat`,
  `memcpy`, `memmove`.
- A shorter string that is a prefix of a longer one sorts first:
  `str_compare("app", "apple")` is negative.
- Comparison is unsigned: `str_compare("\x7f", "\x80")` must be
  negative even on platforms where plain `char` is signed.
- Never read past the terminating `'\0'` of any argument — the grader
  re-runs everything under AddressSanitizer/UBSan, and an out-of-bounds
  read fails the task.

Grade with `ctrain check strings-by-hand`.
