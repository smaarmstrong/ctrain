#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "usage: %s COUNT FILE...\n", argv[0]);
        return 2;
    }

    char *end = NULL;
    errno = 0;
    long count = strtol(argv[1], &end, 10);
    if (errno != 0 || end == argv[1] || *end != '\0' || count <= 0) {
        fprintf(stderr, "usage: COUNT must be a positive integer\n");
        return 2;
    }

    int any_failed = 0;
    for (int i = 2; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (f == NULL) {
            fprintf(stderr, "head: %s: %s\n", argv[i], strerror(errno));
            any_failed = 1;
            continue;
        }
        char buf[1024];
        long lines = 0;
        while (lines < count && fgets(buf, sizeof buf, f) != NULL) {
            fputs(buf, stdout);
            if (strchr(buf, '\n') != NULL)
                lines++;
        }
        fclose(f);
    }
    return any_failed ? 1 : 0;
}
