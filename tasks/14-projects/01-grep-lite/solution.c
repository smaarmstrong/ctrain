#include <stdio.h>
#include <string.h>

static int nflag, cflag, vflag;
static const char *pattern;
static int selected_any;

static void process(FILE *fp, const char *name, int prefix)
{
    char line[4100];
    long lineno = 0, count = 0;

    while (fgets(line, sizeof line, fp)) {
        lineno++;
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';

        int hit = (pattern[0] == '\0') || strstr(line, pattern) != NULL;
        if (vflag)
            hit = !hit;
        if (!hit)
            continue;

        selected_any = 1;
        count++;
        if (!cflag) {
            if (prefix)
                printf("%s:", name);
            if (nflag)
                printf("%ld:", lineno);
            printf("%s\n", line);
        }
    }
    if (cflag) {
        if (prefix)
            printf("%s:", name);
        printf("%ld\n", count);
    }
}

int main(int argc, char **argv)
{
    int i = 1;
    int had_error = 0;

    for (; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0)
            nflag = 1;
        else if (strcmp(argv[i], "-c") == 0)
            cflag = 1;
        else if (strcmp(argv[i], "-v") == 0)
            vflag = 1;
        else
            break;
    }
    if (i >= argc) {
        fprintf(stderr, "usage: greplite [-n] [-c] [-v] PATTERN [FILE...]\n");
        return 2;
    }
    pattern = argv[i++];

    int nfiles = argc - i;
    if (nfiles == 0) {
        process(stdin, "(stdin)", 0);
    } else {
        int prefix = nfiles >= 2;
        for (; i < argc; i++) {
            FILE *fp = fopen(argv[i], "r");
            if (!fp) {
                fprintf(stderr, "greplite: %s: cannot open\n", argv[i]);
                had_error = 1;
                continue;
            }
            process(fp, argv[i], prefix);
            fclose(fp);
        }
    }

    if (had_error)
        return 2;
    return selected_any ? 0 : 1;
}
