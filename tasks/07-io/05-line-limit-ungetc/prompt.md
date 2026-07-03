# line_limited and next_int

Implement these two functions in `main.c`. Do **not** define `main` — the
grader links your file against a test harness that provides it.

```c
#include <stdio.h>
#include <stddef.h>

long line_limited(FILE *stream, char *buf, size_t cap);
int next_int(FILE *stream, int *out);
```

## `line_limited` — a bounded line reader

Read one line from `stream` into `buf`:

- Store at most `cap - 1` characters, then a terminating `'\0'` (the
  harness always passes `cap >= 2`, and `buf` really is only `cap` bytes
  — writing past it is an overflow).
- Reading stops after a `'\n'`, which **is stored** when it fits.
- If the line is longer than `cap - 1` characters, stop at `cap - 1`
  stored characters and leave the rest **in the stream**: the next call
  picks up exactly where this one stopped. No input is ever lost.
- Return the number of characters stored, **excluding** the `'\0'`.
- If the stream is at end-of-file before anything could be read, return
  `-1` (and store just `'\0'`). A final line with no trailing newline is
  returned normally; the `-1` comes on the call after it.

Example: with the stream containing `abcdefghij\n` and `cap == 5`,
successive calls return `4` (`"abcd"`), `4` (`"efgh"`), `3` (`"ij\n"`),
then `-1`.

## `next_int` — integer scanning with one-character lookahead

Read the next decimal integer from `stream`:

- Skip any leading whitespace (as classified by `isspace`).
- An optional single `'+'` or `'-'` sign may precede the digits.
- Consume the digits, store the value through `out`, and **push the first
  non-digit character back** onto the stream with `ungetc`, so the caller
  sees it on the next read. Return `1`.
- If the first non-whitespace character is neither a sign nor a digit:
  push **it** back, leave `*out` untouched, return `0`.
- If a sign is not followed by a digit: the sign stays consumed, the
  character after it is pushed back, `*out` is untouched, return `0`.
- If end-of-file is reached before any non-whitespace character: return
  `-1`.

Example: on a stream containing `"  42x-7"` the calls yield: `1` (value
42, next `fgetc` returns `'x'`), then `1` (value −7), then `-1`.

Values always fit in `int` — do not worry about overflow. Use only
standard C stdio (`fgetc`, `ungetc`, …). The harness runs everything a
second time under AddressSanitizer/UBSan: a write past the end of `buf`
fails the task even if the answers look right.

Your code goes in `main.c` in the work directory. Grade with
`ctrain check line-limit-ungetc`.
