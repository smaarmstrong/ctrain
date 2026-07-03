/* Test harness — provides main() and calls the learner's functions. */
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

struct book {
    char   title[32];
    int    year;
    double price;
};

int book_cmp(const void *a, const void *b);
void sort_books(struct book *books, size_t n);
const struct book *find_book(const struct book *books, size_t n,
                             int year, const char *title);

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
    /* --- book_cmp sign checks ------------------------------------------ */
    struct book b1 = { "Alpha", 1990, 1.0 };
    struct book b2 = { "Beta", 2005, 2.0 };
    struct book b3 = { "Alpha", 2005, 3.0 };
    struct book b1dup = { "Alpha", 1990, 9.0 }; /* price is not compared */

    CHECK(book_cmp(&b1, &b2) < 0, "earlier year compares smaller");
    CHECK(book_cmp(&b2, &b1) > 0, "later year compares greater");
    CHECK(book_cmp(&b3, &b2) < 0, "same year: strcmp order on title");
    CHECK(book_cmp(&b2, &b3) > 0, "same year: reverse title order");
    CHECK(book_cmp(&b1, &b1dup) == 0, "same year and title: equal");

    struct book far_past = { "Old", INT_MIN, 0.0 };
    struct book far_future = { "New", INT_MAX, 0.0 };
    CHECK(book_cmp(&far_past, &far_future) < 0 &&
              book_cmp(&far_future, &far_past) > 0,
          "INT_MIN vs INT_MAX years: correct sign (no subtraction overflow)");

    /* --- sort_books ----------------------------------------------------- */
    struct book shelf[] = {
        { "Zebra Types",  2001, 15.00 },
        { "K&R Notes",    1988, 9.99 },
        { "Ada Ideas",    2001, 20.50 },
        { "Micro Ops",    1975, 3.10 },
        { "Yacc Days",    2019, 30.00 },
        { "Bit Fiddling", 1988, 12.00 },
    };
    size_t n = sizeof shelf / sizeof shelf[0];
    sort_books(shelf, n);

    const char *want[] = { "Micro Ops", "Bit Fiddling", "K&R Notes",
                           "Ada Ideas", "Zebra Types", "Yacc Days" };
    int order_ok = 1;
    for (size_t i = 0; i < n; i++)
        if (strcmp(shelf[i].title, want[i]) != 0)
            order_ok = 0;
    CHECK(order_ok, "shelf sorted by year, ties by title");
    if (!order_ok)
        for (size_t i = 0; i < n; i++)
            printf("        [%zu] %d %s\n", i, shelf[i].year, shelf[i].title);

    struct book none[1] = { { "Unused", 0, 0.0 } };
    sort_books(none, 0);
    CHECK(strcmp(none[0].title, "Unused") == 0,
          "sort_books with n == 0 touches nothing");

    /* --- find_book ------------------------------------------------------ */
    const struct book *hit = find_book(shelf, n, 2001, "Ada Ideas");
    CHECK(hit != NULL && hit->price == 20.50,
          "finds a book in a tied year (price confirms identity)");
    hit = find_book(shelf, n, 1975, "Micro Ops");
    CHECK(hit != NULL && hit->price == 3.10, "finds the first element");
    hit = find_book(shelf, n, 2019, "Yacc Days");
    CHECK(hit != NULL && hit->price == 30.00, "finds the last element");
    CHECK(find_book(shelf, n, 2001, "Ada Ideaz") == NULL,
          "present year, absent title -> NULL");
    CHECK(find_book(shelf, n, 1999, "Ada Ideas") == NULL,
          "absent year -> NULL");
    CHECK(find_book(shelf, 0, 1988, "K&R Notes") == NULL, "n == 0 -> NULL");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
