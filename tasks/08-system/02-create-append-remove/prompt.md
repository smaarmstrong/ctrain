# create, append, remove

Implement three small wrappers around the file syscalls in `main.c`.
Do **not** define `main` — the grader links your file against a test
harness that provides it.

Put `#define _POSIX_C_SOURCE 200809L` at the top of your file so the
declarations in `<fcntl.h>` and `<unistd.h>` are visible under
`-std=c11`.

```c
#include <stddef.h>

/* Create the file at path, or truncate it to zero bytes if it already
 * exists. New files get permission bits exactly 0644 (rw-r--r--) —
 * the harness clears the umask, so what you pass to open() is what the
 * file ends up with. The file descriptor must be closed before
 * returning. Return 0 on success, -1 on any error. */
int file_create(const char *path);

/* Append exactly n bytes from buf to the END of an EXISTING file at
 * path. The data is raw bytes — NUL bytes included — not a C string.
 * If the file does not exist, return -1 and do NOT create it.
 * Return 0 when all n bytes were written, -1 on any error.
 * Appending 0 bytes to an existing file succeeds. */
int file_append(const char *path, const void *buf, size_t n);

/* Delete the file at path. Return 0 on success, -1 on error
 * (for example: it does not exist). */
int file_remove(const char *path);
```

Hints: `file_create` wants `O_WRONLY | O_CREAT | O_TRUNC` with an
explicit mode; `file_append` wants `O_WRONLY | O_APPEND` and **no**
`O_CREAT`; `file_remove` is `unlink`. `write` may write fewer bytes
than asked — loop until done. Never return leaving a file descriptor
open, including on error paths.

The harness checks the mode bits with `stat`, appends across two calls
and reads the bytes back, probes the missing-file cases, and runs the
whole thing again under sanitizers.

Grade with `ctrain check create-append-remove`.
