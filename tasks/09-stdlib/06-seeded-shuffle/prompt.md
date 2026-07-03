# deterministic Fisher–Yates with srand/rand

"Random" and "reproducible" are not opposites: seed the generator with a
known value and the whole sequence — and therefore the whole shuffle — is
fixed. That is how tests, simulations and replays stay deterministic.

Write a program invoked as:

```
./prog SEED N
```

- `SEED` — base-10 unsigned integer, passed to `srand` exactly once.
- `N` — base-10 integer, `1 <= N <= 1000`.

The program builds the array `0, 1, 2, …, N-1` and shuffles it with the
**modern Fisher–Yates algorithm, exactly as specified**:

```
srand(SEED);
for i from N-1 down to 1:
    j = rand() % (i + 1)      /* one rand() call per iteration */
    swap a[i] and a[j]
```

Follow the recipe to the letter — loop direction, the modulo expression,
one `rand()` call per step, nothing consuming randomness before or
between. The grader compiles the reference implementation with the same
C library as your program and compares output byte for byte, so any
deviation (an extra `rand()` call, `i` counting upward, `j = rand() % i`)
produces a different permutation and fails.

Output: the shuffled values on **one line**, separated by single spaces,
terminated by one newline. For `N` = 1 that is just `0` and a newline
(the loop body never runs). Nothing else on stdout.

Error handling — exit with status 1 (any diagnostic goes to stderr, not
stdout) when:

- the argument count is not exactly 2, or
- either argument is not a pure base-10 number (use `strtol`/`strtoul`
  with an end pointer — `"12x"` is invalid), or
- `N` is outside 1…1000.

On success exit with status 0.

Example (the exact digits depend on the platform's `rand`; the *shape* is
what matters):

```
$ ./prog 42 8
2 6 0 5 1 7 3 4
$ ./prog 42 8      # same seed, same order — always
2 6 0 5 1 7 3 4
```

Your code goes in `main.c`. Grade with `ctrain check seeded-shuffle`.
