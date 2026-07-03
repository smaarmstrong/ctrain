# squeeze blanks

Write a filter: a program that reads its standard input one character at
a time with `getchar()` and writes to standard output with `putchar()`,
copying everything through unchanged **except** that every run of one or
more space characters (`' '`) is replaced by a single space.

Rules:

- Only the space character is squeezed. Tabs, newlines and everything
  else pass through untouched and do **not** break or join a run of
  spaces on their own — `"a \t x"` contains two separate one-space
  runs and comes out unchanged.
- Runs are squeezed wherever they occur: leading spaces on a line become
  one space, and `"a   b"` becomes `"a b"`.
- Empty input produces empty output.
- Input that does not end in a newline is fine; do not add one.
- Keep reading until `getchar()` returns `EOF`, then exit with status 0.
  Remember: the variable holding `getchar()`'s result must be an `int`,
  not a `char`, or the `EOF` test is unreliable.

Example — input:

```
too   many    spaces
	tab	 stays
```

output:

```
too many spaces
	tab	 stays
```

Your code goes in `main.c` in the work directory. Grade with
`ctrain check squeeze-blanks`.
