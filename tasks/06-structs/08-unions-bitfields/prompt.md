# tagged unions and bit-fields

Two independent parts, both in `main.c`. Do **not** define `main` — the
grader links your file against a test harness that provides it.

Copy these definitions into your file exactly as written (the harness
uses the same definitions and reads the members directly):

```c
#include <stddef.h>

enum vkind { VK_INT, VK_REAL, VK_TEXT };

struct variant {
    enum vkind kind;
    union {
        long i;         /* valid when kind == VK_INT  */
        double r;       /* valid when kind == VK_REAL */
        const char *s;  /* valid when kind == VK_TEXT */
    } as;
};

struct perm {
    unsigned read  : 1;
    unsigned write : 1;
    unsigned exec  : 1;
    unsigned level : 5;   /* 0..31 */
};
```

## Part 1 — a tagged union

```c
/* Constructors: set the tag and the matching union member.
 * var_text stores the pointer itself — no copy is made. */
struct variant var_int(long v);
struct variant var_real(double v);
struct variant var_text(const char *s);

/* Format *v into buf using snprintf(buf, n, ...) with, per kind:
 *   VK_INT   "int:%ld"      e.g. int:42
 *   VK_REAL  "real:%.2f"    e.g. real:2.50
 *   VK_TEXT  "text:%s"      e.g. text:hi
 * Return what snprintf returns: the full formatted length, even when
 * it did not fit in n (snprintf then truncates but still NUL-
 * terminates). n is at least 1. */
int var_describe(const struct variant *v, char *buf, size_t n);
```

The point of the tag: `var_describe` must switch on `v->kind` and read
only the member that the tag says is valid — reading the others is
undefined behaviour that the sanitizer run can catch.

## Part 2 — bit-fields

A permission byte packs five fields into 8 bits: bit 0 (value 1) is
`read`, bit 1 (value 2) is `write`, bit 2 (value 4) is `exec`, and bits
3–7 hold `level` (so `level == (bits >> 3) & 0x1f`).

```c
/* Unpack: bits is always < 256. */
struct perm perm_from_bits(unsigned bits);

/* Pack: exact inverse of perm_from_bits. */
unsigned perm_to_bits(struct perm p);
```

The harness round-trips **all 256** values both ways and also checks the
individual bit-field members, so the mapping must be exact. It then
reruns everything under the sanitizers.

Grade with `ctrain check unions-bitfields`.
