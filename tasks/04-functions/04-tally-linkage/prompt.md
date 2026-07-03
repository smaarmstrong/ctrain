# a tally module with private state

Implement, in `main.c`, a small statistics module that remembers the
values fed to it *between* calls. Do **not** define `main` — the
grader links your file against a test harness that provides it.

```c
#include <stddef.h>

void tally_reset(void);   /* forget everything recorded so far      */
void tally_add(long v);   /* record one value                       */
size_t tally_count(void); /* how many values since the last reset   */
long tally_sum(void);     /* their sum (always fits in a long)      */
long tally_min(void);     /* smallest recorded value; 0 when empty  */
long tally_max(void);     /* largest recorded value; 0 when empty   */
```

Rules the harness enforces:

- The module starts out empty — the queries must work correctly even
  if `tally_reset` has never been called (count 0, sum 0, min 0,
  max 0).
- After `tally_reset`, the module is empty again.
- Values may be negative; `min`/`max` must be right when only one
  value has been recorded.

Since the functions share state across calls, you will need file-scope
variables. Here is the linkage lesson: a file-scope variable without
`static` has **external** linkage — its name is visible to every other
file in the program, and two files defining the same name will not
link. To prove your state is private, the harness itself defines
global variables named `count`, `sum`, `total`, `min` and `max`.
Declare your module's state `static` (internal linkage) and the names
can coexist; leave `static` off state with one of those names and the
program will fail to link.

The harness also checks its own globals are untouched after your
functions run.

Grade with `ctrain check tally-linkage`.
