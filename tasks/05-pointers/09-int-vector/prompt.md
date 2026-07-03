# int vector

A growable array is the workhorse of dynamic memory: a heap buffer, a
length, a capacity, and a growth policy. Build one for `int`.

Copy this struct definition into `main.c` **exactly** (the harness
compiles against the same layout), then implement the four functions.
Do **not** define `main` — the grader links your file against a test
harness that provides it.

```c
#include <stddef.h>

typedef struct {
    int *data;      /* heap buffer, or NULL when cap == 0 */
    size_t len;     /* elements in use */
    size_t cap;     /* elements allocated */
} intvec;

/* Put a fresh, empty vector in *v: data NULL, len 0, cap 0.
 * No allocation happens here. */
void vec_init(intvec *v);

/* Append x. Return 0 on success, -1 if allocation fails (the vector
 * must then be unchanged and still usable).
 * Growth policy — checked by the grader via the cap field:
 *   - first allocation: cap becomes 8
 *   - when full after that: cap exactly DOUBLES (8, 16, 32, ...)
 *   - pushing when there is room must NOT reallocate or change cap
 * Use realloc to grow so the existing elements survive. */
int vec_push(intvec *v, int x);

/* Remove the last element and store it in *out. Return 0, or -1 if
 * the vector is empty (leave *out untouched). Popping never shrinks
 * the buffer or changes cap. */
int vec_pop(intvec *v, int *out);

/* Release the buffer and reset *v to the fresh state (data NULL,
 * len 0, cap 0), so init -> use -> free -> use again is legal.
 * Freeing a fresh vector is a no-op. */
void vec_free(intvec *v);
```

Notes:

- The grader inspects `len`, `cap` and `data` directly, pushes ~20 000
  elements, pops them all back, and re-runs everything under
  AddressSanitizer — a leak, an over-read, or a buffer that didn't
  really grow fails the task.
- `len` and `cap` are counts of **elements**, not bytes. Remember to
  multiply by `sizeof(int)` when allocating.
- After `vec_pop` succeeds, `len` shrinks by one; the popped slot's
  contents need not be cleared.

Grade with `ctrain check int-vector`.
