# Your first Makefile

The work directory contains a tiny two-module calculator — `main.c`,
`ops.c`, `ops.h` — and a placeholder `Makefile` that refuses to build
anything. The C code is finished and correct; **you only write the
Makefile**. Do not modify the C sources.

## What `make` must do

Running plain `make` in the work directory (the **default target**) must
produce an executable named `calc`, built **via object files**: after a
successful build both `main.o` and `ops.o` must exist next to the
sources. Compile with:

```
-std=c11 -Wall -Wextra -Werror -g
```

Put the flags in a `CFLAGS` variable and use a pattern rule
(`%.o: %.c`) rather than writing one compile recipe per file — that is
the skill this task practises. (The grader checks the artifacts and
behaviour, not the Makefile text.)

## Behaviour of the built program

`calc` takes exactly two integer arguments and prints two lines:

```
$ ./calc 6 7
6 + 7 = 13
6 * 7 = 42
```

With any other number of arguments it prints a usage message to stderr
and exits non-zero. All of this is already implemented in the sources —
if your build is correct, the behaviour comes for free.

## Rebuild rules the grader checks

- Immediately after a successful `make`, a second `make` must do **no
  work**: everything is up to date (`make -q` exits 0). Your default
  target must be the real file `calc`, not a phony always-runs rule.
- Each `.o` must depend on `ops.h` as well as its `.c` file (list the
  header as a prerequisite in the pattern rule).

## `make clean`

A `clean` target must delete `calc` and all `.o` files. Declare it
`.PHONY` — the grader creates a stray file named `clean` in the
directory and then runs `make clean`; without `.PHONY`, make decides
"clean is up to date" and your recipe never runs.

## Grading

1. `make` succeeds; `calc`, `main.o`, `ops.o` all exist.
2. `./calc 6 7` and `./calc -3 10` print the exact lines shown above
   (with the obvious numbers); no-argument run exits non-zero.
3. A second `make` has nothing to do.
4. With a decoy file `clean` present, `make clean` still removes the
   binary and every `.o`.
5. `make` after `clean` rebuilds successfully.

Grade with `ctrain check first-makefile`.
