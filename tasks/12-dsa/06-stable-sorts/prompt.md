# insertion sort and a stable mergesort

Implement two sorting functions in `main.c`. Do **not** define `main` —
the grader links your file against a test harness that provides it.

Copy this type and these prototypes into your file exactly as written
(the harness uses the same definitions):

```c
#include <stddef.h>

typedef struct {
    int key;   /* the ONLY field the sort order depends on */
    int tag;   /* payload — must travel with its key, never compared */
} Rec;

void insertion_sort(Rec *a, size_t n);
void merge_sort(Rec *a, size_t n);
```

Required behaviour, for **both** functions:

- Sort `a[0..n)` into ascending order of `key`. The `tag` field is
  payload: it moves with its record and must never influence ordering.
- **Stable**: records with equal keys keep their original relative
  order. The harness tags every record with its input position and
  fails the task if two equal keys ever come out swapped.
- The result is a permutation of the input — no record duplicated,
  lost, or invented.
- `n` may be 0, in which case `a` may be `NULL`; do nothing and do not
  dereference it. A single-element array is already sorted.

Extra requirements per function:

- `insertion_sort` sorts in place and must not allocate.
- `merge_sort` may allocate scratch space, but must free everything it
  allocates before returning (the harness runs leak-checked under
  AddressSanitizer/valgrind). It must run in O(n·log n): the harness
  sorts a 200,000-record array with `merge_sort`, and a quadratic
  algorithm will hit the time limit and fail.

Example: sorting keys `3,1,3,1,2` (tags `0,1,2,3,4` marking input
position) must yield keys `1,1,2,3,3` with tags `1,3,4,0,2` — the two
1s and the two 3s stay in input order.

The harness probes empty, single-element, reverse-sorted, all-equal,
and duplicate-heavy arrays, then repeats everything under
AddressSanitizer/UBSan, so an out-of-bounds write or a leaked scratch
buffer fails the task even if the answers look right.

Grade with `ctrain check stable-sorts`.
