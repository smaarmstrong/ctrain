# echo the arguments

When a program starts, `main(int argc, char *argv[])` receives its
command line: `argc` counts the entries, `argv[0]` is the program name,
and `argv[1] … argv[argc-1]` are the arguments. `argv` is exactly the
structure from the sort-lines task — an array of pointers to strings.

Write a whole program (with `main`) that prints its arguments back:

- Print each argument **after the program name** in order, separated
  by a single space.
- After the last argument print one `'\n'`.
- With **no arguments** print nothing at all — not even a newline.
- Never print `argv[0]`.
- Exit status 0 always.

The output is compared byte-for-byte, so separators matter: no space
before the first argument, no space after the last, exactly one space
between neighbours.

Examples (`$` marks the shell prompt):

```
$ ./prog hello world
hello world
$ ./prog one
one
$ ./prog "two words"   -n   --
two words -n --
$ ./prog
$
```

Note in the third example that `-n` and `--` are ordinary arguments to
you — do not treat anything as an option — and that an argument may
itself contain a space.

Grade with `ctrain check echo-args`.
