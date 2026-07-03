# two builds of the same program

Your work directory contains `main.c` — a tiny integer-square-root tool —
and a `Makefile` that builds it twice... identically, which defeats the
point. **Do not edit `main.c`.** Fix the `Makefile` so the two binaries
actually differ.

`main.c` contains two compile-time switches:

- an `assert(n >= 0 ...)` guarding the input — active by default, compiled
  out when `NDEBUG` is defined;
- a trace line printed to stderr **only when `DEBUG` is defined**.

## Required Makefile behaviour

- `make` (or `make all`) builds **both** binaries:
  - `app-debug` — compiled with `-DDEBUG`, `-O0` and `-g`
  - `app-release` — compiled with `-DNDEBUG` and `-O2`
- `make clean` removes both binaries; `all` and `clean` are declared
  `.PHONY`.
- Both builds use `-std=c11 -Wall -Wextra -Werror`.

## Resulting behaviour (this is what gets graded)

```
$ ./app-debug 49          $ ./app-release 49
debug: parsing '49'       7
7
```
(the trace goes to stderr; both print `7` on stdout)

For a negative input the two builds must part ways:

- `./app-debug -9` — the assertion fires: the process aborts with a
  non-zero status and prints **no** result;
- `./app-release -9` — the assertion is compiled out, the fallback path
  runs: it prints `no result` and exits with status `1`.

Grade with `ctrain check debug-release-builds`.
