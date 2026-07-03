#include <stdio.h>

int main(void)
{
    int c;
    int in_run = 0;

    while ((c = getchar()) != EOF) {
        if (c == ' ') {
            if (!in_run)
                putchar(' ');
            in_run = 1;
        } else {
            putchar(c);
            in_run = 0;
        }
    }
    return 0;
}
