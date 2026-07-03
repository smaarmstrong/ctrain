# cat with raw file descriptors

Write a minimal `cat` that talks to the kernel directly. The whole point
of this task is to stay **below** the standard I/O library:

- **Do not include `stdio.h`** — the grader rejects a source that
  mentions it. No `printf`, `fopen`, `getchar`, `putchar`, …
- Use the syscall wrappers instead: `open`, `read`, `write`, `close`
  from `<fcntl.h>` and `<unistd.h>`.
- Put `#define _POSIX_C_SOURCE 200809L` before your includes so those
  declarations are visible under `-std=c11`.

## Behaviour

```
./prog                # copy standard input (fd 0) to standard output (fd 1)
./prog FILE...        # copy each named file to standard output, in order
```

- With no arguments, read fd 0 until end of input and write everything
  to fd 1.
- With arguments, open each file read-only, copy its contents to fd 1,
  and close it, processing the files strictly in argument order.
- The copy must be **byte-exact and binary-safe**: NUL bytes, bytes ≥
  128 and missing trailing newlines all pass through unchanged. Treat
  data as bytes, never as text.
- Read in decent-sized chunks (a few KiB); handle a short `write` by
  looping until the whole chunk is out.
- Print nothing except the copied bytes on stdout.

## Errors

If a file cannot be opened, write a short diagnostic of your choosing to
**fd 2** (remember: `write(2, ...)`, not `fprintf`), stop, and exit with
status 1. A failed `read` or `write` also means exit status 1. On
success exit 0.

## Example

If `a.txt` holds `one\n` and `b.txt` holds `two\n`:

```
$ ./prog a.txt b.txt
one
two
```

Your code goes in `main.c` in the work directory. Grade with
`ctrain check fd-cat`.
