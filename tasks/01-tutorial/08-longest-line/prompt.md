# longest line of input

Read standard input to `EOF` and report the longest line: print its
length in characters on one line, then the line itself on the next, and
exit with status 0.

```
LENGTH
the line itself
```

Precise rules:

- A line's **length excludes the newline**: `"abc\n"` is a line of
  length 3.
- If the input does not end in a newline, the final unterminated chunk
  still counts as a line: for the input `hi` (two bytes, no newline) the
  output is `2`, newline, `hi`, newline. Your output always ends with a
  newline, even when the winning line had none.
- **Ties go to the first**: for `aa\nbb\n` print `2` then `aa`.
- An empty line can win: the input consisting of a single `\n` prints
  `0` followed by an empty line.
- **Completely empty input (zero bytes) prints nothing** and still
  exits 0.
- No line will be longer than 1000 characters, so a fixed `char`
  array of, say, 1024 is plenty — but you must copy the current best
  line into a second array (or equivalent) because the winner has to
  survive while later lines are read.

Example — input:

```
short
the longest line
mid
```

output:

```
16
the longest line
```

The grader rebuilds your program with AddressSanitizer and UBSan and
runs it again: writing past the end of a buffer or forgetting the
string terminator fails the task even if the output happens to look
right.

Your code goes in `main.c` in the work directory. Grade with
`ctrain check longest-line`.
