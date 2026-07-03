# fixed-size records with lseek

A file is treated as an array of **16-byte records**: record `idx`
occupies bytes `idx*16` through `idx*16 + 15`. Implement random access
to such a file in `main.c`. Do **not** define `main` — the grader
links your file against a test harness that provides it.

Put `#define _POSIX_C_SOURCE 200809L` at the top so `<unistd.h>`
declares `lseek`, `read` and `write` under `-std=c11`.

```c
/* Read record idx from fd into out (exactly 16 bytes).
 * Return 0 on success; -1 if idx is negative, the record is not fully
 * present in the file (reaches past end-of-file), or any syscall
 * fails. */
int record_read(int fd, long idx, unsigned char *out);

/* Write the 16 bytes at rec as record idx of fd, overwriting whatever
 * was there. Writing past the current end of file is allowed and
 * extends the file (the kernel makes any skipped-over gap read back as
 * zero bytes). Return 0 on success; -1 if idx is negative or any
 * syscall fails. */
int record_write(int fd, long idx, const unsigned char *rec);

/* Return the number of COMPLETE records in fd: file size divided by
 * 16, rounding down (a trailing partial record does not count).
 * Return -1 if the size cannot be determined. */
long record_count(int fd);
```

Rules the harness relies on:

- Functions must work in any call order and never assume where the
  file offset currently is — position explicitly with `lseek` every
  time. They may leave the offset anywhere afterwards.
- A `read` or `write` may transfer fewer bytes than requested; loop
  until the full 16 bytes are done. A `read` that hits end-of-file
  mid-record means the record is not fully present: return -1.
- `record_count` is the file size in bytes (hint: `lseek(fd, 0,
  SEEK_END)`) divided by 16, ignoring any remainder.
- Do not close `fd` — the harness owns it.

The harness writes records out of order, reads them back out of order,
overwrites one and checks its neighbours, probes negative and
past-the-end indices, writes beyond EOF and reads the zero-filled gap,
then repeats everything under sanitizers.

Grade with `ctrain check record-seek`.
