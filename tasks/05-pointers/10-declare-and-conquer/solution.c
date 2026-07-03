#include <stddef.h>

static int add(int a, int b) { return a + b; }
static int sub(int a, int b) { return a - b; }
static int mul(int a, int b) { return a * b; }
static int divi(int a, int b) { return a / b; }

int (*ops[4])(int, int) = {add, sub, mul, divi};

int (*get_op(char op))(int, int)
{
    switch (op) {
    case '+': return ops[0];
    case '-': return ops[1];
    case '*': return ops[2];
    case '/': return ops[3];
    default:  return NULL;
    }
}

int (*pick_row(int (*m)[4], size_t nrows, size_t r))[4]
{
    if (r >= nrows)
        return NULL;
    return &m[r];
}
