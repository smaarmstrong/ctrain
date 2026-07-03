# Warning hunt: make it -Werror-clean

`main.c` is a small parts-inventory report. It runs and prints the right
numbers — but it does **not** compile under the flags this trainer grades
with:

```
cc -std=c11 -Wall -Wextra -Werror -g -o prog main.c
```

Your job is to read each diagnostic the compiler emits and fix the code it
points at, **without changing what the program prints**. This is a
warnings exercise, not a rewrite: every warning marks a genuine blemish
(an unused parameter, an unused variable, a signed/unsigned comparison, a
`printf` conversion that doesn't match its argument's type). Fix the code
properly — do not silence anything with pragmas or by loosening the flags,
and do not delete the helper function or its call.

## Required behaviour

The program takes no input and no arguments. After your fixes it must
compile cleanly with the command above and print exactly:

```
bolt     x12  = 420
washer   x40  = 200
bracket  x3   = 747
rail     x2   = 2400
parts: 4 grand total: 3767
```

(Every line ends with a newline; there is no trailing blank line beyond
the final newline. Exit status 0.)

## Grading

1. Compiles with `-std=c11 -Wall -Wextra -Werror` — any warning fails.
2. Output is byte-for-byte identical to the listing above.

## Hints

- Compile with the exact flags shown; read the *first* warning, fix it,
  recompile. Warnings often cascade.
- `sizeof` yields an unsigned `size_t` — pick the right loop index type.
- `printf` conversions must match the argument type exactly: `%d` is for
  `int`, `%ld` for `long`.
- A deliberately unused parameter can be kept quiet with `(void)name;`,
  but if a variable serves no purpose at all, remove it.
