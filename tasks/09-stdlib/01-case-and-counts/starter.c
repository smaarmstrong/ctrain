#include <ctype.h>
#include <stdio.h>

int main(void)
{
    /* TODO:
     *   - read stdin with getchar() until EOF
     *   - echo each byte with letter case swapped (isupper/islower,
     *     toupper/tolower) — remember the unsigned char cast
     *   - count letters / digits / spaces / other with isalpha, isdigit,
     *     isspace, tested in that order
     *   - finally print: letters=L digits=D spaces=S other=O
     */
    int c;
    while ((c = getchar()) != EOF)
        putchar(c);
    printf("letters=0 digits=0 spaces=0 other=0\n");
    return 0;
}
