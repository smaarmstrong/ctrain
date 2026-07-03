# sorted directory listing with sizes

Write a program that lists one directory, with a size for every
regular file — a tiny, deterministic `ls -l`.

Put `#define _POSIX_C_SOURCE 200809L` at the top so `<dirent.h>` and
`<sys/stat.h>` declare what you need under `-std=c11`. Plain stdio
(`printf` etc.) is fine in this task.

## Behaviour

```
./prog DIR
```

- Read the entries of `DIR` with `opendir`/`readdir`/`closedir`.
- Skip **only** the entries named `.` and `..`. Other dot-files
  (`.hidden`) are listed like anything else.
- `readdir` returns entries in arbitrary order, so **sort the names**
  by plain byte order (`strcmp`) before printing.
- `stat` each entry (build the path as `DIR/name`) and print one line
  per entry to stdout:
  - regular file: `SIZE NAME` — the size in bytes, in decimal,
  - directory: `dir NAME`,
  - anything else: `other NAME`,

  with a single space between the two fields and no other padding.
- An empty directory prints nothing.
- Exit status 0 on success.

## Errors

If the argument count is wrong, or `DIR` cannot be opened, or an entry
cannot be stat'ed, print a message to **stderr** and exit with a
non-zero status.

## Example

For a directory holding a 5-byte file `notes.txt`, an empty file
`empty`, and a subdirectory `src`:

```
$ ./prog thatdir
0 empty
5 notes.txt
dir src
```

Keep memory clean: the grader reruns your program under sanitizers,
so every allocation made while collecting names must be freed before
exit.

Your code goes in `main.c` in the work directory. Grade with
`ctrain check list-dir`.
