# bounded_copy

An *out-of-bounds access* — reading or writing even one byte past the
end of an array — is undefined behaviour. In practice it silently
corrupts whatever happens to live next in memory, which is why
off-by-one errors in "safe" copy routines are a classic source of
security bugs.

The starter code in `main.c` contains one such routine. It looks
reasonable, it compiles cleanly, and it is wrong: in some situations it
writes **one byte past the end** of the destination buffer. Your job is
to find the bug and fix the function so it honours this contract
exactly:

```c
#include <stddef.h>

/* Copy src into dst, never writing more than dst_cap bytes total.
 *
 *  - If dst_cap == 0: write NOTHING to dst (not even a '\0') and
 *    return 0. dst may not point at any usable space at all.
 *  - Otherwise: copy at most dst_cap - 1 characters of src, then
 *    always terminate dst with '\0'. The terminator counts toward
 *    dst_cap, so the last writable byte is dst[dst_cap - 1].
 *  - Return the number of characters copied, NOT counting the '\0'.
 */
size_t bounded_copy(char *dst, size_t dst_cap, const char *src);
```

Examples:

- `bounded_copy(buf, 10, "hi")` writes `h`, `i`, `\0` and returns 2.
- `bounded_copy(buf, 4, "hello")` writes `h`, `e`, `l`, `\0` and
  returns 3 — the result is truncated to fit.
- `bounded_copy(buf, 1, "hello")` writes only `\0` and returns 0.
- `bounded_copy(buf, 0, "hello")` writes nothing and returns 0.

Do **not** define `main` — the grader links your file against a test
harness. The harness plants sentinel bytes immediately after the space
it hands you and fails the task if any sentinel changes; it then
repeats every call with exactly-sized heap buffers under
AddressSanitizer, where a single stray byte aborts the run.

Grade with `ctrain check bounded-copy`.
