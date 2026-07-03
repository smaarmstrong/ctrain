# Parse with care

`scanf("%d %d", ...)` looks like the obvious way to read pairs of numbers,
but it treats a newline as just more whitespace: it will happily pair up a
number from one line with a number from the next, and when it hits
something that isn't a number it leaves the junk in the buffer, where it
jams every later read. The robust pattern is to read whole **lines** with
`fgets` and then pick them apart with `sscanf`.

Write a program that reads standard input line by line. Each line is
supposed to contain **exactly two integers** separated by whitespace, with
nothing else on the line apart from optional leading/trailing whitespace.

- For every **valid** line, print to standard output:

  ```
  sum = S
  ```

  where `S` is the decimal sum of the two integers.

- For every **invalid** line — too few numbers, too many, or any
  non-numeric junk such as `12abc` — print to **standard error**:

  ```
  line N: bad input
  ```

  where `N` is the 1-based line number, then carry on with the next line.
  Nothing is printed to standard output for an invalid line.

- A number must live entirely on its own line: a line containing only `1`
  is invalid even if the next line contains only `2`. (This is exactly the
  case where raw `scanf` gets it wrong.)

- Exit with status **0** if every line was valid, **1** if any line was
  bad. Empty input (no lines at all) is fine: print nothing, exit 0.

Example — given this input:

```
3 4
10 -2
1 2 3
foo 9
   5    6
```

stdout is:

```
sum = 7
sum = 8
sum = 11
```

stderr is:

```
line 3: bad input
line 4: bad input
```

and the exit status is 1.

You may assume no input line exceeds 200 characters. Integers are small
enough to fit in `int`, and may be negative. Hint: `sscanf` returns how
many conversions succeeded, and a trailing `%n` records how many
characters were consumed — useful for spotting junk after the numbers.

Your code goes in `main.c` in the work directory. Grade with
`ctrain check parse-with-care`.
