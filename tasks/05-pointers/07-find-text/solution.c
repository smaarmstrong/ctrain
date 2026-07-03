#include <stdio.h>
#include <string.h>

#define MAXLINE 1024

static void usage(void)
{
    fprintf(stderr, "usage: findtext [-v] [-n] pattern\n");
}

int main(int argc, char *argv[])
{
    int invert = 0, number = 0;
    int i = 1;

    while (i < argc && argv[i][0] == '-' && argv[i][1] != '\0') {
        if (strcmp(argv[i], "-v") == 0)
            invert = 1;
        else if (strcmp(argv[i], "-n") == 0)
            number = 1;
        else {
            usage();
            return 2;
        }
        i++;
    }
    if (i != argc - 1) {
        usage();
        return 2;
    }
    const char *pattern = argv[i];

    char line[MAXLINE];
    long lineno = 0, printed = 0;
    while (fgets(line, sizeof line, stdin) != NULL) {
        lineno++;
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';
        int hit = strstr(line, pattern) != NULL;
        if (hit != invert) {
            if (number)
                printf("%ld:%s\n", lineno, line);
            else
                printf("%s\n", line);
            printed++;
        }
    }
    return printed > 0 ? 0 : 1;
}
