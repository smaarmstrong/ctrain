# a buffered reader over read(2)

`getchar` feels like it reads one byte at a time, but underneath, stdio
fetches a big block with one `read` syscall and doles it out from a
buffer. Build that layer yourself in `main.c`. Do **not** define
`main` — the grader links your file against a test harness that
provides it.

Put `#define _POSIX_C_SOURCE 200809L` at the top of your file.

```c
struct breader; /* your definition — the harness treats it as opaque */

/* Allocate reader state for fd on the heap and return it.
 * Return NULL if allocation fails. The reader does not own fd. */
struct breader *br_open(int fd);

/* Return the next byte as an int in 0..255, or -1 at end of input
 * (and on any read error). Once -1 has been returned, every further
 * call also returns -1. Bytes >= 128 must come back as positive
 * values — beware of storing them in plain char. */
int br_getc(struct breader *b);

/* Push one byte back: the very next br_getc returns c, then reading
 * continues where it left off. c does not have to be the byte that
 * was last read, and pushback also works after end of input. Only ONE
 * pending byte must be supported: if a pushed-back byte is already
 * waiting, or c is not in 0..255, store nothing and return -1.
 * On success return c. */
int br_ungetc(struct breader *b, int c);

/* Free everything br_open allocated. Does NOT close the fd. */
void br_close(struct breader *b);
```

## The buffering requirement (checked!)

Your internal buffer must be **at least 512 bytes**, and `br_getc` must
fetch data through the **`read` syscall wrapper** (`unistd.h`), not
through stdio. The grader intercepts `read` at link time and counts the
calls your code makes while consuming an 8192-byte file byte-by-byte:

- at least 1 call (using `fread`/`getc` instead scores 0 and fails),
- at most 24 calls (a 512-byte buffer refilled on demand needs 17).

Reading one byte per syscall fails this check.

The harness also verifies: every byte of a patterned 8 KiB file comes
back intact and in order; `-1` repeats at end of input; the pushback
rules above; an empty file returns `-1` immediately. Everything then
runs again under ASan/UBSan, so the state you allocate must be freed
by `br_close` and never over-read.

Grade with `ctrain check buffered-reader`.
