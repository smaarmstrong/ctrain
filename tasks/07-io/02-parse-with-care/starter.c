#include <stdio.h>

int main(void)
{
    /* This "works" on clean input but has all of scanf's classic
     * problems: it pairs numbers across lines, and one piece of junk
     * silently ends the whole run. Rewrite it with fgets + sscanf. */
    int a, b;
    while (scanf("%d %d", &a, &b) == 2)
        printf("sum = %d\n", a + b);
    return 0;
}
