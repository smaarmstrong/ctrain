# Count Collatz steps

The Collatz step takes a positive integer `n` to `n / 2` when `n` is
even, and to `3n + 1` when `n` is odd. Repeating the step from any
starting value tested so far eventually reaches 1.

Write a program that reads integers from standard input until end of
file (whitespace-separated) and, for each value `n` in input order,
prints one line:

- if `n >= 1`: the number of steps needed to reach 1, formatted as

  ```
  n: s
  ```

  Starting at 1 takes 0 steps.

- if `n < 1`: the step is not defined, so print

  ```
  n: invalid
  ```

  and carry on with the next value.

Sample: input `6 1 0 7` produces

```
6: 8
1: 0
0: invalid
7: 16
```

Print nothing else; every line ends with a newline; empty input produces
no output.

Two loops are in play: an outer read loop and an inner stepping loop.
Either can be a `while` or a `for` — the two forms are mechanically
interchangeable (`for (;cond;)` *is* `while (cond)`); pick whichever
reads better, and try rewriting one into the other once it works.

Inputs are at most 1,000,000, but intermediate values along the way can
overflow a 32-bit `int` — hold the changing value in a `long long`.

Your code goes in `main.c` in the work directory. Grade with
`ctrain check collatz-steps`.
