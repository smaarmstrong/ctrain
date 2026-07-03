# precedence traps: fix what the compiler saw

Someone wrote four tiny functions from the specs below, leaving out
parentheses and trusting their intuition about operator precedence.
Their intuition was wrong four times. The `main.c` starter you receive
is that code with the grouping the compiler *actually* used made
explicit — every pair of parentheses in it shows you exactly how C
parsed the original. All four functions compile cleanly and all four
compute the wrong thing.

Fix each function so it matches its spec. Do **not** define `main` —
the grader links your file against a test harness that provides it.

The specs:

```c
#include <stdbool.h>

/* True iff lo <= x AND x <= hi.
 * (The original wrote the maths-style lo <= x <= hi; C instead
 * compares the 0-or-1 result of lo <= x against hi.) */
bool between(int lo, int x, int hi);

/* True iff the low 4 bits of x equal v.
 * (The original wrote x & 0xF == v; == binds tighter than &.) */
bool low_nibble_is(unsigned x, unsigned v);

/* a shifted left by 2, then b added: (a << 2) + b.
 * (The original wrote a << 2 + b; + binds tighter than <<.)
 * The grader only uses small non-negative a and b, so overflow
 * is not a concern here. */
int shifted_sum(int a, int b);

/* base, plus 10 more when bonus is set: base + (bonus ? 10 : 0).
 * (The original wrote base + bonus ? 10 : 0; ?: binds almost
 * loosest of all, so the addition landed in the condition.) */
int bonus_total(int base, bool bonus);
```

Keep the function names and signatures exactly as they are. Any
correct implementation passes — you may rewrite the bodies entirely
rather than just moving parentheses.

Grade with `ctrain check precedence-traps`.
