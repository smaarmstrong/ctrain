# Integer to decimal string with a do-while

Implement this function in `main.c`. Do **not** define `main` — the
grader links your file against a test harness that provides it.

```c
/* Write the decimal representation of n into out as a NUL-terminated
 * string: an optional leading '-', then the digits, no padding, no
 * leading zeros (except "0" itself).
 * The caller guarantees out has room for at least 12 bytes — exactly
 * enough for "-2147483648" plus the terminating NUL. */
void int_to_decimal(int n, char out[]);
```

Rules and edge cases:

- Build the string yourself, digit by digit. Do **not** call `sprintf`
  or `snprintf` (the grader rejects submissions containing either name).
- `n == 0` must produce `"0"` — this is why a `do-while` is the natural
  loop here: the body must run at least once, so zero gets its digit
  without a special case.
- Negative values get a single leading `'-'`.
- `INT_MIN` (−2147483648) must work. Beware: computing `-n` when
  `n == INT_MIN` overflows `int`, which is undefined behaviour and will
  be caught by the sanitizer. Either work in a wider type (`long long`)
  or accumulate on the negative side.
- Write only within the first 12 bytes of `out`; never read or write
  past them. The harness hands you a buffer of exactly 12 bytes and
  re-runs everything under AddressSanitizer/UBSan, so an off-by-one or
  a signed overflow fails the task even if the strings look right.

Examples of exact results: `0` → `"0"`, `42` → `"42"`, `-5` → `"-5"`,
`2147483647` → `"2147483647"`, `-2147483648` → `"-2147483648"`.

Grade with `ctrain check int-to-decimal`.
