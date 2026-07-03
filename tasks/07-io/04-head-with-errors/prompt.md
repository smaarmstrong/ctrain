# head with proper error handling

Write a simplified `head` that takes a line count followed by one or more
file names:

```
prog COUNT FILE...
```

For each file, in argument order, print its first `COUNT` lines to
standard output. Files are simply concatenated — no headers or separators
between them. A file with fewer than `COUNT` lines is printed whole. A
final line without a trailing newline still counts as a line and is
printed as-is.

The point of this task is the **error handling**:

- **Usage errors** — fewer than two arguments, or a `COUNT` that is not a
  positive decimal integer (`0`, `-3`, `abc`, `7x` are all invalid):
  print a single line beginning with `usage:` to **standard error** and
  exit with status **2**. Print nothing to standard output.

- **File errors** — a file that cannot be opened: print a diagnostic to
  **standard error** in the classic `perror` shape:

  ```
  head: FILENAME: REASON
  ```

  where `REASON` is the system's description of `errno` (what
  `strerror(errno)` returns — e.g. `No such file or directory`). Then
  **continue with the remaining files**. If any file failed, the final
  exit status is **1**.

- **Success** — every file opened and printed: exit status **0**.

Standard output must carry *only* file contents — never diagnostics, and
never partial output for a file that failed to open.

Example: with `good.txt` present (5 lines) and `nope.txt` absent,
`prog 2 good.txt nope.txt good.txt` prints lines 1–2 of `good.txt` twice
to stdout, prints `head: nope.txt: No such file or directory` to stderr,
and exits 1.

You may assume no line in the test files exceeds 500 characters. Parse
`COUNT` strictly — `strtol` and a check that the whole argument was
consumed is one clean way.

Your code goes in `main.c` in the work directory. Grade with
`ctrain check head-with-errors`.
