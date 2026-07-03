/* Test harness — provides main() and calls the learner's functions. */
#include <stddef.h>
#include <stdio.h>
#include <string.h>

enum vkind { VK_INT, VK_REAL, VK_TEXT };

struct variant {
    enum vkind kind;
    union {
        long i;
        double r;
        const char *s;
    } as;
};

struct perm {
    unsigned read  : 1;
    unsigned write : 1;
    unsigned exec  : 1;
    unsigned level : 5;
};

struct variant var_int(long v);
struct variant var_real(double v);
struct variant var_text(const char *s);
int var_describe(const struct variant *v, char *buf, size_t n);
struct perm perm_from_bits(unsigned bits);
unsigned perm_to_bits(struct perm p);

static int failures;

#define CHECK(cond, msg)                                        \
    do {                                                        \
        if (cond) {                                             \
            printf("  ok    %s\n", msg);                        \
        } else {                                                \
            printf("  FAIL  %s\n", msg);                        \
            failures++;                                         \
        }                                                       \
    } while (0)

int main(void)
{
    /* --- part 1: tagged union ------------------------------------- */
    struct variant vi = var_int(42);
    CHECK(vi.kind == VK_INT && vi.as.i == 42, "var_int sets tag and value");

    struct variant vr = var_real(2.5);
    CHECK(vr.kind == VK_REAL && vr.as.r == 2.5, "var_real sets tag and value");

    const char *msg = "hi";
    struct variant vt = var_text(msg);
    CHECK(vt.kind == VK_TEXT && vt.as.s == msg,
          "var_text stores the pointer itself");

    char buf[32];
    int len = var_describe(&vi, buf, sizeof buf);
    CHECK(len == 6 && strcmp(buf, "int:42") == 0, "describe int:42");

    len = var_describe(&vr, buf, sizeof buf);
    CHECK(len == 9 && strcmp(buf, "real:2.50") == 0, "describe real:2.50");

    struct variant vh = var_text("hello");
    len = var_describe(&vh, buf, sizeof buf);
    CHECK(len == 10 && strcmp(buf, "text:hello") == 0, "describe text:hello");

    struct variant vneg = var_int(-7);
    len = var_describe(&vneg, buf, sizeof buf);
    CHECK(len == 6 && strcmp(buf, "int:-7") == 0, "describe a negative int");

    /* truncation: snprintf semantics */
    struct variant big = var_int(1234567);
    memset(buf, 'X', sizeof buf);
    len = var_describe(&big, buf, 6);
    CHECK(len == 11, "truncated describe still returns the full length (11)");
    CHECK(strcmp(buf, "int:1") == 0, "truncated output is NUL-terminated at n-1");

    /* --- part 2: bit-fields --------------------------------------- */
    struct perm p = perm_from_bits(0xEDu); /* 1110 1101: r=1 w=0 x=1 level=29 */
    CHECK(p.read == 1 && p.write == 0 && p.exec == 1,
          "perm_from_bits(0xED) unpacks r, w, x");
    CHECK(p.level == 29, "perm_from_bits(0xED) unpacks level 29");

    struct perm q = perm_from_bits(0);
    CHECK(q.read == 0 && q.write == 0 && q.exec == 0 && q.level == 0,
          "perm_from_bits(0) is all zeroes");
    CHECK(perm_to_bits(q) == 0, "perm_to_bits of all zeroes is 0");

    struct perm h;
    h.read = 0; h.write = 1; h.exec = 1; h.level = 20;
    CHECK(perm_to_bits(h) == 0xA6u, "perm_to_bits packs w+x, level 20 as 0xA6");

    int ok = 1;
    for (unsigned b = 0; b < 256; b++) {
        struct perm t = perm_from_bits(b);
        if (perm_to_bits(t) != b)
            ok = 0;
        if (t.read != (b & 1u) || t.write != ((b >> 1) & 1u) ||
            t.exec != ((b >> 2) & 1u) || t.level != ((b >> 3) & 0x1fu))
            ok = 0;
    }
    CHECK(ok, "all 256 permission bytes round-trip exactly");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
