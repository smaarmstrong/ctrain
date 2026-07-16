# make basics

The work directory contains a finished `main.c` (do not change it) and a
`Makefile` that is nothing but comments. Complete the Makefile with one
rule, exactly as taught in the lesson:

- target `greet`, built from `main.c`,
- recipe: compile with `cc -Wall -Werror -o greet main.c`,
- remember: the recipe line starts with a real Tab character.

The grader will drive your Makefile the way you would:

- plain `make` must build an executable `./greet` as its default target,
- `./greet` must print exactly `built by make, so never stale` and a
  newline, exiting 0,
- immediately after a build, `make -q` must report nothing to do
  (exit 0),
- after `touch main.c`, `make -q` must report a rebuild is needed
  (exit non-zero), and `make` must rebuild successfully.

That behaviour all comes free from one correctly written rule — no
tricks needed.

Edit `Makefile` in the work directory (main.c is already correct).
Grade with `ctrain check make-basics`.
