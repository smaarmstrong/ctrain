# string stack — who owns this memory?

Two of the nastiest heap bugs are *use-after-free* (touching memory
after it was handed back to the allocator — the allocator may have
recycled or scribbled on it) and *double free* (freeing the same
pointer twice — this corrupts the allocator's bookkeeping, and modern
libc versions abort the process when they catch it). Both are undefined
behaviour, and both are almost always **ownership** bugs: two parties
each believing they are responsible for freeing the same block.

`main.c` contains a small stack of strings. It compiles cleanly and
looks plausible, but it breaks the ownership rules below in **two**
places. Find both bugs and fix them. Do **not** define `main` — the
grader links your file against a test harness.

```c
#include <stddef.h>

typedef struct {
    char **items;   /* array of cap pointers; the first count are live */
    size_t count;
    size_t cap;
} sstack;

/* Put *s into the empty state: items NULL, count 0, cap 0. */
void sstack_init(sstack *s);

/* Push a COPY of str (the stack must not keep a pointer into the
 * caller's buffer). Grow the array as needed.
 * Return 0 on success, -1 if an allocation fails. */
int sstack_push(sstack *s, const char *str);

/* Remove the top string and return it. From that moment the CALLER
 * owns the returned string and will free() it; the stack must not
 * keep or free it. Return NULL if the stack is empty. */
char *sstack_pop(sstack *s);

/* Free every string still on the stack and the items array itself,
 * then reset *s to the empty state (items NULL, count 0, cap 0).
 * Calling sstack_destroy again on an already-destroyed stack must be
 * safe and do nothing. The stack must be reusable after destroy. */
void sstack_destroy(sstack *s);
```

Symptoms you can reproduce with the harness before fixing anything:
a popped string comes back as garbage instead of its pushed contents,
and the program aborts inside `free()`. Under AddressSanitizer the
grader additionally requires the whole session to be free of
use-after-free, double-free, and leak reports — a `destroy` that
forgets strings still on the stack fails too.

Grade with `ctrain check string-stack-ownership`.
