# find-text

Write a whole program (with `main`): a miniature `grep` that prints
the lines of standard input containing a given piece of text.

## Command line

```
findtext [-v] [-n] pattern
```

- Zero or more **flags** come first, then exactly one `pattern`
  argument.
  - `-v` — invert: print the lines that do **not** contain the pattern.
  - `-n` — number: prefix each printed line with its 1-based line
    number in the *input* and a colon, e.g. `3:the line`.
  - Flags are separate arguments and may appear in either order;
    both may be given. You do not need to support combined flags
    like `-vn`.
- Any of these is a **usage error**: no pattern, more than one
  pattern, or an argument starting with `-` that is not `-v` or `-n`
  (a lone `-` counts as a pattern, not a flag). On usage error print
  exactly this line to **stderr** and exit with status **2**:

  ```
  usage: findtext [-v] [-n] pattern
  ```

## Matching

- A line matches when the pattern occurs in it as a substring
  (`strstr` is allowed and encouraged). The empty pattern `""`
  matches every line.
- Matching is case-sensitive.
- Every input line in the tests is shorter than 1024 characters. If
  the input does not end with `'\n'`, the trailing characters still
  form a final line; print it with a trailing `'\n'` if it is
  selected.
- Print selected lines in input order, each followed by one `'\n'`,
  nothing else on stdout.

## Exit status

- `0` — at least one line was printed
- `1` — the run was fine but no line was printed
- `2` — usage error (see above)

## Example

Input:

```
the cat sat
on the mat
dogs bark
```

`findtext -n the` prints:

```
1:the cat sat
2:on the mat
```

`findtext -v -n the` prints `3:dogs bark`; `findtext zebra` prints
nothing and exits 1.

The grader also re-runs the program under AddressSanitizer/UBSan:
buffer overruns or leaks fail the task.

Grade with `ctrain check find-text`.
