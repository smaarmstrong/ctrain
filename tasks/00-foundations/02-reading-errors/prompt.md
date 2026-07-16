# Reading compiler errors

The starter `main.c` is a tiny receipt printer that does not compile. It
contains exactly three mistakes — the three classics:

- a missing semicolon,
- a misspelled (undeclared) identifier,
- a printf format that does not match its argument (a warning, but
  -Werror makes it fatal here, as it is in every grader in this repo).

Fix them the disciplined way: compile, read the FIRST error only, fix
that one thing, recompile. Repeat until the compiler is silent:

```sh
cc -Wall -Werror -o receipt main.c
```

Do not restructure the program — the three fixes are each a few
characters. When it compiles cleanly it must print exactly:

```
3 apples
total: 75 pence
```

and exit with status 0.

Your code goes in `main.c` in the work directory. Grade with
`ctrain check reading-errors`.
