# the sum that stayed at zero

The starter program is supposed to keep a running total of the
integers it reads, but something is wrong: every line reports a bogus
total, and the final total is always zero. The compiler is perfectly
happy with the code — the bug is a *scope* problem, not a syntax
problem.

In C, a declaration inside a block (`{ ... }`) creates a **new**
variable that hides — *shadows* — any outer variable of the same name
until the block ends. Find where that happens in the starter and fix
the program so it behaves as specified below. (Deleting one line is
enough, but any correct program passes.)

## Required behaviour

- Read integers from standard input with `scanf("%d", ...)` (or any
  equivalent) until end of input.
- After reading each value, print one line:

  ```
  add V -> total T
  ```

  where `V` is the value just read and `T` is the running total
  *including* `V`.
- After input ends, print exactly one final line:

  ```
  final total T over C values
  ```

  where `T` is the sum of all values and `C` is how many were read.
  With no input at all, that line is `final total 0 over 0 values`
  (the word `values` is used even when `C` is 1).

Values may be negative. Print nothing else. Example — input
`3 4 -2 10` produces:

```
add 3 -> total 3
add 4 -> total 7
add -2 -> total 5
add 10 -> total 15
final total 15 over 4 values
```

Exit with status 0.

Grade with `ctrain check shadowed-sum`.
