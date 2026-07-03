#include <stdio.h>

int main(void)
{
    long ndigit[10] = {0};
    long nwhite = 0, nother = 0;
    int c;

    while ((c = getchar()) != EOF) {
        if (c >= '0' && c <= '9')
            ndigit[c - '0']++;
        else if (c == ' ' || c == '\t' || c == '\n')
            nwhite++;
        else
            nother++;
    }

    printf("digits:");
    for (int i = 0; i < 10; i++)
        printf(" %ld", ndigit[i]);
    printf("\nwhite: %ld\nother: %ld\n", nwhite, nother);
    return 0;
}
