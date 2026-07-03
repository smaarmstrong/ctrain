# leak hunt

Your work directory contains `main.c` — a program called `revlines` that is
supposed to:

1. read every line from standard input (each line is newline-terminated and
   at most 1023 bytes including the newline),
2. print the lines back in **reverse order**,
3. finish with a summary line: `-- N lines` (N = number of lines read).

Example — given this input:

```
alpha
beta
gamma
```

it must print:

```
gamma
beta
alpha
-- 3 lines
```

Empty input prints just `-- 0 lines`.

The version you've been handed has **two defects**:

- one you can see by running it — compare its output with the spec above;
- one that is invisible in the output and only shows up under a memory
  checker. Build it with `-fsanitize=address` (or run it under
  `valgrind --leak-check=full`) and read the report.

Fix both. The grader compiles your file normally *and* with sanitizers: a
version that prints the right answer but leaks does not pass.

Grade with `ctrain check leak-hunt`.
