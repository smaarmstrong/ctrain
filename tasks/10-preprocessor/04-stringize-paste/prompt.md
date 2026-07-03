# Stringize, paste, and variadic logging

Two preprocessor operators exist only inside macro bodies: `#` turns a
macro argument into a string literal, and `##` glues two tokens into
one. Both come with a trap: they operate on the argument **as written**,
*before* any macro expansion. Getting the expanded form requires one
extra level of indirection through a helper macro.

Define these four macros in `main.c`. Do **not** define `main` — the
grader textually `#include`s your file into a harness (which includes
`<stdio.h>` first) and checks the program's exact output.

- **`STR(x)`** — expands to a string literal holding the **fully
  macro-expanded** spelling of `x`. Given `#define ANSWER 42`,
  `STR(ANSWER)` must be `"42"` (a plain `#x` would give `"ANSWER"`).
  With `#define GEN 2`, `STR(GEN + GEN)` must be `"2 + 2"` — expanded
  tokens, separated by single spaces.

- **`JOIN(a, b)`** — pastes `a` and `b` into a single identifier
  **after** expanding both arguments. With `#define GEN 2`,
  `JOIN(width_v, GEN)` must name the identifier `width_v2` (a plain
  `a##b` would produce `width_vGEN`).

- **`SHOW(expr)`** — a statement (used as `SHOW(apples + pears);`) that
  prints the argument **exactly as written** (not expanded, tokens
  separated by single spaces), then ` = `, then its value as a decimal
  `int`, then a newline. `SHOW(apples + pears)` with `apples == 3` and
  `pears == 4` prints:

  ```
  apples + pears = 7
  ```

  The harness only passes side-effect-free `int` expressions.

- **`LOG(fmt, ...)`** — printf-style variadic logging, used as a
  statement. Prints `[log] ` (prefix, one trailing space), then behaves
  like `printf(fmt, ...)`, then prints exactly one newline. It must also
  work with a format string alone: `LOG("startup");` prints
  `[log] startup` plus the newline.

The harness runs, among the same kinds of probes shown above:

```c
#define ANSWER 42
#define GEN 2
printf("%s\n", STR(ANSWER));          /* 42          */
printf("%d\n", JOIN(width_v, GEN));   /* value of width_v2 */
LOG("loaded %d items from %s", 7, "cache");
                                      /* [log] loaded 7 items from cache */
```

The starter contains a naive attempt at each macro; all four compile but
print the wrong thing. Helper macros are welcome (they are the whole
point).

Grade with `ctrain check stringize-paste`.
