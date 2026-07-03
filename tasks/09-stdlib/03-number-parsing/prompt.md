# parse numbers properly

`atoi` cannot report failure. `strtol` and `strtod` can — if you use every
part of their interface: the `endptr` out-parameter *and* `errno`. Wrap
them once, correctly, and you never write this code again.

Implement these two functions in `main.c`. Do **not** define `main` — the
grader links your file against a test harness that provides it.

```c
/* Status codes (plain ints, use the literal values): */
/*   0  PARSE_OK     parsed, *out was set                              */
/*  -1  PARSE_BAD    not a number, or junk before/after it             */
/*  -2  PARSE_RANGE  well-formed but out of range for the result type  */

/* Parse s as a base-10 long. */
int parse_long(const char *s, long *out);

/* Parse s as a double (any spelling strtod accepts: 3.5, -2e10, .5). */
int parse_double(const char *s, double *out);
```

Exact rules, both functions:

- The number may be surrounded by whitespace on either side (`isspace`);
  `"  -7  "` is OK. Anything else before or after it is `PARSE_BAD`.
- If no conversion happens at all (empty string, only whitespace,
  `"abc"`) — `PARSE_BAD`. Use `endptr` to detect this: after the call it
  still equals the input pointer.
- A well-formed number the type cannot represent — `PARSE_RANGE`. Detect
  it via `errno == ERANGE` after the call. A malformed string is
  `PARSE_BAD` even if its leading digits would also overflow
  (`"999…999xyz"` is `PARSE_BAD`).
- **Set `errno = 0` before calling** `strtol`/`strtod` — these functions
  never clear it, so a stale `ERANGE` left by earlier code would leak into
  your answer. The harness deliberately dirties `errno` before one of its
  calls to catch exactly this bug.
- On success return `0` and store the value through `out`. On any failure
  return the error code and leave `*out` alone.
- `s` is never NULL; base is always 10 for `parse_long`.

Examples:

| call                                   | returns | *out |
|----------------------------------------|---------|------|
| `parse_long("42", &v)`                 | 0       | 42   |
| `parse_long("  -7  ", &v)`             | 0       | -7   |
| `parse_long("12abc", &v)`              | -1      | —    |
| `parse_long("", &v)`                   | -1      | —    |
| `parse_long("99999999999999999999", &v)` | -2    | —    |
| `parse_double("2.5e3", &d)`            | 0       | 2500 |
| `parse_double("1e999", &d)`            | -2      | —    |

Grade with `ctrain check number-parsing`.
