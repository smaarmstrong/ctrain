#include <stdio.h>

#define MAX_PRODUCTS 100
#define NAME_MAX_LEN 31

struct product {
    char name[NAME_MAX_LEN + 1];
    int total;
};

int main(void)
{
    struct product tally[MAX_PRODUCTS];
    int nproducts = 0;
    (void)tally;

    /* TODO: read "name qty" pairs; look each name up in tally[],
     * add to its total or append a new entry; then print the report. */

    printf("distinct %d\n", nproducts);
    return 0;
}
