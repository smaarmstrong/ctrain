# assert the contract, validate the input

`assert` and an `if` both check a condition, but they answer different
questions. An **assert** documents a *contract with other programmers*:
"calling me like this is a bug in the caller". It aborts loudly in debug
builds and vanishes entirely when the program is compiled with `-DNDEBUG`.
**Runtime validation** guards against *untrusted input*: it must keep
working in every build, so it can never be an assert.

This task grades both — and grades that you put each check in the right
place. The grader compiles your code **twice**: once normally, once with
`-DNDEBUG`, and expects different behaviour from the two binaries.

Implement these two functions in `main.c`. Do **not** define `main` — the
grader links your file against a test harness that provides it.

```c
/* Clamp x into [lo, hi].
 *
 * PRECONDITION (a contract, so: assert it): lo <= hi.
 * - Build WITHOUT NDEBUG: calling clamped(x, 5.0, 2.0) must ABORT the
 *   process (that is what a failed assert does).
 * - Build WITH -DNDEBUG: the check must disappear — the same call must
 *   NOT abort or exit; return any double you like.
 * Use assert(), or your own check guarded by #ifndef NDEBUG; an
 * unconditional abort()/exit() fails the NDEBUG build. */
double clamped(double x, double lo, double hi);

/* Parse an untrusted TCP-port string. VALID means: s is non-NULL,
 * non-empty, consists ONLY of decimal digits (no sign, no spaces), and
 * its value v satisfies 1 <= v <= 65535. Leading zeros are allowed
 * ("0080" -> 80). Return the value, or -1 for anything invalid —
 * including absurdly long digit strings ("99999999999999999999" -> -1;
 * beware of overflowing while you accumulate).
 *
 * This is INPUT, not a contract: it must behave identically in both
 * builds. If you reject bad ports with assert, the -DNDEBUG binary
 * will happily accept them — and fail the grader. */
int parse_port(const char *s);
```

Summary of what is checked, per build:

| check                                   | normal build | -DNDEBUG build |
|-----------------------------------------|--------------|----------------|
| `clamped(3,0,10)`=3, `(-1,0,10)`=0, `(11,0,10)`=10, `(5,5,5)`=5 | yes | — |
| `clamped(1.0, 5.0, 2.0)` aborts          | must abort   | must NOT abort |
| `parse_port` accepts/rejects correctly   | yes          | yes (identical)|

The harness is also run under AddressSanitizer/UBSan (normal build,
valid inputs only).

Grade with `ctrain check assert-contracts`.
