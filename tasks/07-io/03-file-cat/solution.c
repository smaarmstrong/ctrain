#include <stdio.h>

static void copy_stream(FILE *in)
{
    char buf[512];
    while (fgets(buf, sizeof buf, in) != NULL)
        fputs(buf, stdout);
}

int main(int argc, char *argv[])
{
    if (argc == 1) {
        copy_stream(stdin);
        return 0;
    }
    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (f == NULL) {
            fprintf(stderr, "mycat: cannot open %s\n", argv[i]);
            return 1;
        }
        copy_stream(f);
        fclose(f);
    }
    return 0;
}
