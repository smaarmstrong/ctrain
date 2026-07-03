#include <stddef.h>
#include <stdio.h>

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

struct variant var_int(long v)
{
    /* TODO */
    (void)v;
    struct variant out = {VK_INT, {0}};
    return out;
}

struct variant var_real(double v)
{
    /* TODO */
    (void)v;
    struct variant out = {VK_REAL, {0}};
    return out;
}

struct variant var_text(const char *s)
{
    /* TODO */
    (void)s;
    struct variant out = {VK_TEXT, {0}};
    return out;
}

int var_describe(const struct variant *v, char *buf, size_t n)
{
    /* TODO: switch on v->kind; snprintf(buf, n, ...); return its result. */
    (void)v;
    if (n > 0)
        buf[0] = '\0';
    return 0;
}

struct perm perm_from_bits(unsigned bits)
{
    /* TODO: bit 0 read, bit 1 write, bit 2 exec, bits 3-7 level. */
    (void)bits;
    struct perm p = {0, 0, 0, 0};
    return p;
}

unsigned perm_to_bits(struct perm p)
{
    /* TODO: exact inverse of perm_from_bits. */
    (void)p;
    return 0;
}
