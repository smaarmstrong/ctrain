#include <stdio.h>
#include <string.h>

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

    char name[NAME_MAX_LEN + 1];
    int qty;
    while (scanf("%31s %d", name, &qty) == 2) {
        int i;
        for (i = 0; i < nproducts; i++)
            if (strcmp(tally[i].name, name) == 0)
                break;
        if (i == nproducts) {
            if (nproducts == MAX_PRODUCTS)
                continue; /* guaranteed not to happen */
            strcpy(tally[i].name, name);
            tally[i].total = 0;
            nproducts++;
        }
        tally[i].total += qty;
    }

    for (int i = 0; i < nproducts; i++)
        printf("%s %d\n", tally[i].name, tally[i].total);
    printf("distinct %d\n", nproducts);
    return 0;
}
