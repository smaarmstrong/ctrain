# declare and conquer

C declarations are read inside-out: start at the name, go right when
you can (`()` function, `[]` array), left when you must (`*` pointer),
respecting parentheses. This task makes you *write* three of the
classics and *use* them, because the harness calls them for real.

Define the following in `main.c`, with exactly these types and names.
Do **not** define `main` — the grader links your file against a test
harness that declares these `extern` and provides `main()`.

## 1. An array of pointers to functions

```c
int (*ops[4])(int, int);
```

Read it: `ops` is an array of 4 pointers to functions taking
`(int, int)` and returning `int`. Fill it (at file scope or from
anywhere before use — a file-scope initializer with your own named
helper functions is the natural way) so that:

- `ops[0](a, b)` returns `a + b`
- `ops[1](a, b)` returns `a - b`
- `ops[2](a, b)` returns `a * b`
- `ops[3](a, b)` returns `a / b` (integer division; the harness never
  passes `b == 0`)

## 2. A function that returns a function pointer

```c
int (*get_op(char op))(int, int);
```

Read it: `get_op` is a function taking `char`, returning a pointer to
a function taking `(int, int)` and returning `int`. It must map
`'+' '-' '*' '/'` to **the same pointers stored in `ops[0..3]`** (the
harness compares them with `==`) and return `NULL` for any other
character.

## 3. A function that returns a pointer to an array

```c
int (*pick_row(int (*m)[4], size_t nrows, size_t r))[4];
```

Read it: `pick_row` takes `m` — a pointer to an array of 4 `int`,
i.e. how a 2-D array `int m[nrows][4]` arrives at a function — plus a
row count and a row index. It returns a pointer to row `r` (so
`pick_row(m, n, r) == &m[r]` and `(*pick_row(m, n, r))[c]` is
`m[r][c]`), or `NULL` when `r >= nrows`. Include `<stddef.h>` for
`size_t`.

Notes:

- You may use `typedef`s internally to keep your sanity, as long as
  the three names above end up with exactly the types shown (the
  harness's `extern` declarations must match what you define).
- No allocation is needed anywhere; `pick_row` returns a pointer into
  the caller's matrix, never fresh memory.
- The grader re-runs everything under AddressSanitizer/UBSan.

Grade with `ctrain check declare-and-conquer`.
