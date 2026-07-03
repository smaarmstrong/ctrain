# Split into modules

The work directory holds `main.c`, a single-file program that prints one
line of statistics about an embedded dataset, plus a one-rule `Makefile`
that compiles it directly. It works — but everything lives in one file,
and any edit recompiles the world.

Refactor it into a proper multi-file layout **without changing what the
program prints**, and upgrade the Makefile so it rebuilds only what
changed.

## Required layout (all files in the work directory root)

- `stats.h` — the module's public interface, with an include guard,
  declaring exactly these three functions:

  ```c
  #include <stddef.h>

  /* Sum of the first n ints; 0 when n is 0. */
  long sum_i(const int *a, size_t n);

  /* Largest of the first n ints; the caller guarantees n >= 1. */
  int max_i(const int *a, size_t n);

  /* Arithmetic mean of the first n ints; the caller guarantees n >= 1. */
  double mean_i(const int *a, size_t n);
  ```

- `stats.c` — the implementations. It must **not** define `main`: the
  grader links `stats.c` against its own test harness, so a `main` here
  is a duplicate-symbol link error.
- `main.c` — keeps the dataset and `main`, calls the module through
  `stats.h`.
- `Makefile` — builds the executable `statcalc` from `main.o` and
  `stats.o`.

## Program behaviour (unchanged)

`./statcalc` takes no input and prints exactly one line:

```
n=8 sum=2470 max=997 mean=308.75
```

(mean printed with `%.2f`; exit status 0.)

## Incremental-build rules the grader checks

After a full build the grader back-dates all files, touches one, reruns
`make`, and inspects which objects got newer:

- touch **`stats.c`** → `stats.o` is recompiled, `main.o` is **not**;
- touch **`stats.h`** → **both** `main.o` and `stats.o` are recompiled.

So both object files must list `stats.h` as a prerequisite (write the
dependencies by hand or generate them — either is fine).

## Module correctness

The grader also compiles `stats.c` together with its own harness
(`-I` pointing at your directory, so your `stats.h` is what gets
included) and exercises the three functions directly, including
`sum_i(NULL, 0)` — which must return 0 without touching the pointer.
The harness build is repeated under AddressSanitizer/UBSan: any
out-of-bounds read in your loops fails the task.

## Grading

1. `make` succeeds; `statcalc`, `main.o`, `stats.o` exist; output is the
   exact line above.
2. The two incremental-rebuild probes described above.
3. Harness link + run against your `stats.c`/`stats.h`, then again under
   sanitizers.

Grade with `ctrain check split-into-modules`.
