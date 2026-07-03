# greplite

Write a small fixed-string grep:

```
greplite [-n] [-c] [-v] PATTERN [FILE...]
```

It prints every input line that contains PATTERN as a plain substring
(no regular expressions).

## Argument parsing

- Each option is its own argument (`-n -v`, never `-nv`) and options may
  appear in any order, but always before the pattern.
- The first argument that is not exactly `-n`, `-c` or `-v` is the
  PATTERN. Everything after it is a FILE name.
- If no PATTERN is present at all, print a usage message to **stderr**
  and exit with status 2.
- An empty pattern (`""`) matches every line.

## Options

- `-n` — prefix each printed line with its 1-based line number and a
  colon: `3:the lazy dog`.
- `-c` — print only the number of selected lines, one number per input,
  instead of the lines themselves (print `0` if nothing matched).
  When `-c` is given, `-n` has no effect.
- `-v` — select the lines that do **not** contain PATTERN. `-c` counts
  the selected (i.e. non-matching) lines.

## Files and stdin

- With no FILE arguments, read standard input.
- With one FILE, print matching lines exactly as they appear in the file.
- With **two or more** FILE arguments, prefix every output line with the
  file name as given on the command line and a colon. The prefix order
  is `FILE:LINENO:line` when `-n` is active, and `-c` prints one
  `FILE:COUNT` line per file, in command-line order.
- A file that cannot be opened gets a one-line message on **stderr**;
  keep processing the remaining files. (The two-or-more rule counts file
  *arguments*, including unopenable ones.)

Example — `greplite -n the poem.txt` where `poem.txt` holds four lines:

```
1:the quick brown fox
3:the lazy dog
```

## Output details

- Every printed line ends with a single `\n`, including a final input
  line that had no trailing newline in the file.
- Nothing but selected lines / counts goes to stdout; all diagnostics go
  to stderr.
- Input lines are guaranteed to be at most 4096 characters long, so a
  fixed `fgets` buffer is fine.

## Exit status

- `0` — at least one line was selected (across all inputs).
- `1` — the run was error-free but nothing was selected.
- `2` — usage error (missing PATTERN) or any file could not be opened,
  even if other files produced matches.

Your code goes in `main.c` in the work directory (you may split it into
several `.c` files; all of them are compiled together). Grade with
`ctrain check grep-lite`.
