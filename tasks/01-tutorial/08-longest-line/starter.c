#include <stdio.h>

#define MAXLINE 1024

int main(void)
{
    /* TODO: accumulate the current line in a char array; when it ends,
     * compare its length with the best so far and copy it if longer.
     * Watch the two tricky cases: a last line with no '\n', and
     * completely empty input (print nothing). */
    int c;
    while ((c = getchar()) != EOF)
        ;
    return 0;
}
