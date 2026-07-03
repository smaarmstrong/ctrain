# Feature flags with conditional compilation

One source file, several products: the build system passes `-D` flags on
the compiler command line and the preprocessor selects what gets
compiled. Write `main.c` so that the **same file** builds every variant
below.

The program prints exactly three lines and exits with status 0:

```
edition: community
items: 4
logging: off
```

The lines respond to macros that may (or may not) be defined on the
command line:

- **`PRO`** — if the macro `PRO` is defined (any value, or no value), the
  first line is `edition: pro`; otherwise it is `edition: community`.
- **`MAX_ITEMS`** — the second line prints the value of `MAX_ITEMS` as a
  decimal integer. When the build does not define it, default it to `4`
  inside the file (use `#ifndef`). So `-DMAX_ITEMS=9` prints `items: 9`;
  no flag prints `items: 4`.
- **`ENABLE_LOG`** — if defined, the third line is `logging: on`;
  otherwise `logging: off`.

Two configurations are invalid and must **refuse to compile** via
`#error` (any error message you like):

- `PRO` and `TRIAL` both defined — the trial build of the pro edition
  does not exist.
- `MAX_ITEMS` less than 1 (e.g. `-DMAX_ITEMS=0`) — test this with `#if`,
  which works because `MAX_ITEMS` is a macro.

Example: `cc -std=c11 -DPRO -DENABLE_LOG -DMAX_ITEMS=9 main.c` builds a
program that prints:

```
edition: pro
items: 9
logging: on
```

Print nothing else. The grader compiles your file with several flag
combinations, runs the resulting programs, and verifies that the two
invalid configurations fail to compile.

Grade with `ctrain check feature-flags`.
