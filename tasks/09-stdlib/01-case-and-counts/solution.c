#include <ctype.h>
#include <stdio.h>

int main(void)
{
    unsigned long letters = 0, digits = 0, spaces = 0, other = 0;
    int c;

    while ((c = getchar()) != EOF) {
        unsigned char u = (unsigned char)c;

        if (isalpha(u))
            letters++;
        else if (isdigit(u))
            digits++;
        else if (isspace(u))
            spaces++;
        else
            other++;

        if (isupper(u))
            putchar(tolower(u));
        else if (islower(u))
            putchar(toupper(u));
        else
            putchar(u);
    }

    printf("letters=%lu digits=%lu spaces=%lu other=%lu\n",
           letters, digits, spaces, other);
    return 0;
}
