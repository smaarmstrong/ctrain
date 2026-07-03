# search, split, erase — a string.h toolkit

Implement these three functions in `main.c`. Do **not** define `main` —
the grader links your file against a test harness that provides it.

```c
#include <stddef.h>

/* Number of NON-overlapping occurrences of needle in hay.
 * After a match, scanning resumes past the whole match, so
 * count_occurrences("aaaa", "aa") is 2, not 3.
 * An empty needle occurs 0 times by definition here.
 * hay and needle are never NULL. */
size_t count_occurrences(const char *hay, const char *needle);

/* Split line IN PLACE at commas, strtok-style: runs of consecutive
 * commas, and commas at either end, produce no empty fields.
 * Store a pointer to each field (a NUL-terminated string inside the
 * original buffer) into fields[0..], but never store more than max.
 * Return the number of pointers stored.
 *   "one,two,three" max 8 -> 3: "one" "two" "three"
 *   "a,,b"          max 8 -> 2: "a" "b"
 *   ",,,"           max 8 -> 0
 *   "a,b,c,d"       max 2 -> 2: "a" "b"
 * line is writable and never NULL; max may be 0 (store nothing). */
size_t split_fields(char *line, char *fields[], size_t max);

/* Delete count bytes starting at position pos from the string s,
 * in place, closing the gap. The harness guarantees pos <= strlen(s);
 * if count reaches past the end of the string, delete to the end.
 *   erase_range("hello world", 5, 1) -> "helloworld"
 *   erase_range("abc", 1, 99)        -> "a"
 *   erase_range(s, strlen(s), k)     -> s unchanged
 * The regions involved OVERLAP: pick your copy function accordingly
 * (this is exactly the memcpy vs memmove distinction — under the
 * sanitizers an overlapping memcpy fails the task). */
void erase_range(char *s, size_t pos, size_t count);
```

Any correct implementation passes: `strstr`, `strchr`, `strtok`, and
`memmove` make each of these a few lines, but hand-rolled loops are fine
too — as long as they are exact and memory-safe. The whole harness is run
a second time under AddressSanitizer and UBSan: out-of-bounds reads,
overlapping `memcpy`, or a missing NUL terminator fail the task even when
the visible answers look right.

Grade with `ctrain check string-toolkit`.
