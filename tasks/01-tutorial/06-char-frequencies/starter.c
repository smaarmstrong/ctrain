#include <stdio.h>

int main(void)
{
    long ndigit[10] = {0};
    long nwhite = 0, nother = 0;

    /* TODO: read to EOF, bumping ndigit[c - '0'], nwhite or nother. */

    printf("digits:");
    for (int i = 0; i < 10; i++)
        printf(" %ld", ndigit[i]);
    printf("\nwhite: %ld\nother: %ld\n", nwhite, nother);
    return 0;
}
