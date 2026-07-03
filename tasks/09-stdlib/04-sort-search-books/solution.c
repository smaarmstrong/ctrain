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
    const struct book *pa = a;
    const struct book *pb = b;

    if (pa->year != pb->year)
        return pa->year < pb->year ? -1 : 1;
    return strcmp(pa->title, pb->title);
}

void sort_books(struct book *books, size_t n)
{
    qsort(books, n, sizeof *books, book_cmp);
}

const struct book *find_book(const struct book *books, size_t n,
                             int year, const char *title)
{
    struct book key = { .year = year };
    size_t i = 0;

    while (i < sizeof key.title - 1 && title[i] != '\0') {
        key.title[i] = title[i];
        i++;
    }
    key.title[i] = '\0';

    return bsearch(&key, books, n, sizeof *books, book_cmp);
}
