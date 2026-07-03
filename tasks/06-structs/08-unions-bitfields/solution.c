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
    struct variant out;
    out.kind = VK_INT;
    out.as.i = v;
    return out;
}

struct variant var_real(double v)
{
    struct variant out;
    out.kind = VK_REAL;
    out.as.r = v;
    return out;
}

struct variant var_text(const char *s)
{
    struct variant out;
    out.kind = VK_TEXT;
    out.as.s = s;
    return out;
}

int var_describe(const struct variant *v, char *buf, size_t n)
{
    switch (v->kind) {
    case VK_INT:
        return snprintf(buf, n, "int:%ld", v->as.i);
    case VK_REAL:
        return snprintf(buf, n, "real:%.2f", v->as.r);
    case VK_TEXT:
        return snprintf(buf, n, "text:%s", v->as.s);
    }
    if (n > 0)
        buf[0] = '\0';
    return 0;
}

struct perm perm_from_bits(unsigned bits)
{
    struct perm p;
    p.read = bits & 1u;
    p.write = (bits >> 1) & 1u;
    p.exec = (bits >> 2) & 1u;
    p.level = (bits >> 3) & 0x1fu;
    return p;
}

unsigned perm_to_bits(struct perm p)
{
    return p.read | (p.write << 1) | (p.exec << 2) | (p.level << 3);
}
