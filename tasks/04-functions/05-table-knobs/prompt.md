# compile-time table knobs

Write a whole program in `main.c` that prints a table of squares. The
twist: the table's bounds are **compile-time knobs**, set with macros
rather than variables, so the same source file can be rebuilt to
produce different tables.

## Behaviour

For every integer `n` from `TABLE_LO` to `TABLE_HI` inclusive
(ascending, step 1), print one line:

```
n n*n
```

— the value, one space, its square, nothing else. Built with the
defaults (`TABLE_LO` = 1, `TABLE_HI` = 10) the full output is:

```
1 1
2 4
3 9
4 16
5 25
6 36
7 49
8 64
9 81
10 100
```

Exit with status 0.

## The knobs

`TABLE_LO` and `TABLE_HI` must be object-like macros that default to
`1` and `10` but can be overridden from the compiler command line.
The grader rebuilds your file several ways:

- plain — expects the 1..10 table above;
- `-DTABLE_LO=3 -DTABLE_HI=6` — expects the 3..6 table;
- `-DTABLE_LO=-2 -DTABLE_HI=2` — expects the -2..2 table (squares of
  negative values are positive).

The trick is to define each macro **only when it is not already
defined**:

```c
#ifndef TABLE_LO
#define TABLE_LO 1
#endif
```

A bare `#define TABLE_LO 1` would collide with the `-D` flag and, with
warnings-as-errors, break the override builds. You may assume
`TABLE_LO <= TABLE_HI` and that every square fits in an `int`.

Grade with `ctrain check table-knobs`.
