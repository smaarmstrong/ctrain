#include <stdio.h>

int main(void)
{
    char line[512];
    int lineno = 0;
    int any_bad = 0;

    while (fgets(line, sizeof line, stdin) != NULL) {
        lineno++;
        int a, b, used = -1;
        if (sscanf(line, "%d %d %n", &a, &b, &used) == 2 &&
            used >= 0 && line[used] == '\0') {
            printf("sum = %d\n", a + b);
        } else {
            fprintf(stderr, "line %d: bad input\n", lineno);
            any_bad = 1;
        }
    }
    return any_bad ? 1 : 0;
}
