# sort and search a shelf of books

`qsort` and `bsearch` are generic: they know nothing about your types and
call back into a comparator you supply. Write one comparator and drive
both with it.

Copy this struct definition into your `main.c` **exactly** (the harness
compiles against an identical definition), then implement the three
functions. Do **not** define `main`.

```c
#include <stddef.h>

struct book {
    char   title[32];   /* NUL-terminated */
    int    year;
    double price;
};

/* qsort/bsearch comparator over two struct book elements.
 * Order: year ascending; ties broken by title ascending (strcmp order).
 * Return <0, 0, >0. Years may be ANY int, including INT_MIN and
 * INT_MAX — the classic `a->year - b->year` trick overflows; compare,
 * don't subtract. */
int book_cmp(const void *a, const void *b);

/* Sort books[0..n-1] with qsort using book_cmp. n may be 0. */
void sort_books(struct book *books, size_t n);

/* books[0..n-1] is already sorted by book_cmp (the harness sorts with
 * YOUR sort_books first). Return a pointer to the element matching both
 * year and title, or NULL if there is none. Use bsearch: build a key
 * struct book and reuse book_cmp. title is never NULL and always fits
 * in the title field. n may be 0. */
const struct book *find_book(const struct book *books, size_t n,
                             int year, const char *title);
```

What the harness checks:

- `book_cmp` returns the right *sign* for pairs differing in year, pairs
  tied on year but differing in title, and equal pairs — including one
  pair with years `INT_MIN` and `INT_MAX`.
- `sort_books` produces the exact expected order on a shuffled shelf that
  contains a year tie.
- `find_book` returns a pointer to the correct element (verified through
  its price), and NULL both for a missing title in a present year and for
  a missing year, and for `n == 0`.

Everything is run again under AddressSanitizer/UBSan: a comparator that
overflows or reads past a struct fails the task.

Grade with `ctrain check sort-search-books`.
