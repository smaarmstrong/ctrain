# windows onto an unsigned

Hardware registers, file permissions, compressed encodings — all of
them pack several small values into one word. This exercise builds the
three primitives for working with a *bit field*: a window of `n`
consecutive bits inside an `unsigned int`.

Conventions used throughout (and by the harness):

- Bit 0 is the least significant bit.
- The field selected by `(pos, n)` is bits `pos` through `pos + n - 1`
  inclusive.
- The grader only calls these with `0 <= pos`, `1 <= n`, and
  `pos + n <= 32` — but the full-width case `pos == 0, n == 32` **is**
  exercised. Remember that shifting a 32-bit value by 32 is undefined
  behaviour, so build your masks a way that never needs a shift
  count of 32.

Implement these in `main.c`. Do **not** define `main` — the grader
links your file against a test harness that provides it.

```c
/* The field's value, shifted down to start at bit 0.
 * get_bits(0xABCD, 4, 8) == 0xBC. */
unsigned get_bits(unsigned x, int pos, int n);

/* x with the field replaced by the low n bits of v; every bit of x
 * outside the field, and every bit of v above its low n bits, is
 * ignored/preserved as you'd expect.
 * set_bits(0xFFFF, 4, 8, 0x12) == 0xF12F. */
unsigned set_bits(unsigned x, int pos, int n, unsigned v);

/* x with every bit in the field inverted and all other bits intact.
 * toggle_bits(0x00F0, 4, 4) == 0x0000. */
unsigned toggle_bits(unsigned x, int pos, int n);
```

The harness checks values first, then rebuilds with UBSan and runs
everything again — an oversized shift fails the task even on inputs
where the wrong mask happens to produce the right answer.

Grade with `ctrain check bit-windows`.
