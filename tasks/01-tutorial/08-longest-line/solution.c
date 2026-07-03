#include <stdio.h>

#define MAXLINE 1024

int main(void)
{
    char line[MAXLINE];
    char longest[MAXLINE];
    int len = 0;
    int max = -1;
    int c;

    longest[0] = '\0';
    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            if (len > max) {
                max = len;
                line[len] = '\0';
                for (int i = 0; i <= len; i++)
                    longest[i] = line[i];
            }
            len = 0;
        } else if (len < MAXLINE - 1) {
            line[len++] = (char)c;
        }
    }
    if (len > 0 && len > max) { /* final line without a trailing newline */
        max = len;
        line[len] = '\0';
        for (int i = 0; i <= len; i++)
            longest[i] = line[i];
    }
    if (max >= 0)
        printf("%d\n%s\n", max, longest);
    return 0;
}
