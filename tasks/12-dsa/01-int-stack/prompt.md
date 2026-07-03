# an array-backed stack

Implement a fixed-capacity stack of `int`s in `main.c`. Do **not** define
`main` — the grader links your file against a test harness that provides it.

Copy this type and these prototypes into your file exactly as written (the
harness uses the same definitions):

```c
#include <stdbool.h>
#include <stddef.h>

#define STACK_CAP 16

typedef struct {
    int    data[STACK_CAP];
    size_t len;
} IntStack;

void   stack_init(IntStack *s);
size_t stack_size(const IntStack *s);
bool   stack_push(IntStack *s, int v);
bool   stack_pop(IntStack *s, int *out);
bool   stack_peek(const IntStack *s, int *out);
```

Required behaviour:

- `stack_init` makes the stack empty. It may be called again on a used
  stack to reset it.
- `stack_size` returns the number of elements currently stored.
- `stack_push` places `v` on top and returns `true`. When the stack
  already holds `STACK_CAP` elements it returns `false` and changes
  nothing.
- `stack_pop` copies the top element into `*out`, removes it, and returns
  `true`. On an empty stack it returns `false` and must not touch `*out`.
- `stack_peek` copies the top element into `*out` without removing it and
  returns `true`; on an empty stack it returns `false` and must not touch
  `*out`.

The harness checks LIFO order (the most recently pushed value pops
first), size bookkeeping after every operation, the full-stack and
empty-stack rejections above, and interleaved push/pop sequences. It then
repeats everything under AddressSanitizer/UBSan, so an out-of-bounds
index into `data` fails the task even if the answers look right.

Grade with `ctrain check int-stack`.
