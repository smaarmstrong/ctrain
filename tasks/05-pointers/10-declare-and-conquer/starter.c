#include <stddef.h>

/* 1. ops: an array of 4 pointers to int(int, int) — currently all
 * NULL. Write four small helper functions and initialize the array
 * with their names. */
int (*ops[4])(int, int);

/* 2. get_op: takes a char, returns a pointer to int(int, int). */
int (*get_op(char op))(int, int)
{
    /* TODO: '+', '-', '*', '/' -> the matching ops entry; else NULL. */
    (void)op;
    return NULL;
}

/* 3. pick_row: takes a pointer to an array of 4 ints (a matrix row),
 * returns a pointer of the same type — or NULL if r is out of range. */
int (*pick_row(int (*m)[4], size_t nrows, size_t r))[4]
{
    /* TODO */
    (void)m;
    (void)nrows;
    (void)r;
    return NULL;
}
