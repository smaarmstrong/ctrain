# a Frac typedef in lowest terms

Implement exact-fraction arithmetic in `main.c`. Do **not** define `main`
— the grader links your file against a test harness that provides it.

Copy this typedef into your file exactly as written (the harness uses the
same definition and reads the fields directly):

```c
typedef struct {
    long num;   /* numerator — carries the sign */
    long den;   /* denominator — always positive */
} Frac;
```

A Frac is **canonical** when `den > 0` and `num`/`den` share no common
factor greater than 1. The value zero is always represented as `0/1`.
Every Frac your functions **return** must be canonical; every Frac they
**receive** already is (the harness builds its inputs with your
`frac_make`).

```c
/* Build the canonical Frac with value num/den.
 * den is never 0. Either argument may be negative: frac_make(1, -2)
 * and frac_make(-1, 2) are both -1/2. frac_make(0, -7) is 0/1. */
Frac frac_make(long num, long den);

/* Exact sum a + b, canonical. */
Frac frac_add(Frac a, Frac b);

/* Exact product a * b, canonical. */
Frac frac_mul(Frac a, Frac b);

/* Compare by value: -1 if a < b, 0 if equal, 1 if a > b. */
int frac_cmp(Frac a, Frac b);
```

Everything is passed and returned by value. Inputs stay small: no
intermediate product exceeds a few thousand, so `long` arithmetic never
overflows. A helper computing the gcd (Euclid's algorithm) will do most
of the work — remember to normalise the sign onto the numerator.

Grade with `ctrain check fraction-typedef`.
