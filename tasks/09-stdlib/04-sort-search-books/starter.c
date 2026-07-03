#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct book {
    char   title[32];
    int    year;
    double price;
};

int book_cmp(const void *a, const void *b)
{
    /* TODO: cast to const struct book *, compare year (no subtraction
     * trick — it overflows), break ties with strcmp on title. */
    (void)a;
    (void)b;
    return 0;
}

void sort_books(struct book *books, size_t n)
{
    /* TODO: one qsort call. */
    (void)books;
    (void)n;
}

const struct book *find_book(const struct book *books, size_t n,
                             int year, const char *title)
{
    /* TODO: fill a struct book key, then one bsearch call with book_cmp. */
    (void)books;
    (void)n;
    (void)year;
    (void)title;
    return NULL;
}
