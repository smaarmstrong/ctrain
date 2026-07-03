# int_pow — a first function

So far everything has lived inside `main`. Now write your first
standalone function, in `main.c`:

```c
long int_pow(int base, int exp);
```

It returns `base` raised to the power `exp`, computed with integer
arithmetic — a loop of multiplications is exactly right. Do **not** call
`pow()` from the maths library; its floating-point answers are not what
we want here.

Contract:

- `exp` is always `>= 0`; you never need to handle negative exponents.
- `int_pow(anything, 0)` is `1` — including `int_pow(0, 0)`.
- `base` may be negative: `int_pow(-2, 3)` is `-8`, `int_pow(-2, 8)`
  is `256`.
- Every result the grader asks for fits in a `long` (the largest is
  `2^62`), so accumulate in a `long`.
- Exponents stay small (at most 62); a simple O(exp) loop is fine.

Do **not** define `main` — the grader links your file against a test
harness that provides it and calls `int_pow` with the cases above and
more.

Grade with `ctrain check integer-power`.
