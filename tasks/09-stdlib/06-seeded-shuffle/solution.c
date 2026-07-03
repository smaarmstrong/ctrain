#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "usage: %s SEED N\n", argv[0]);
        return 1;
    }

    char *end;

    errno = 0;
    unsigned long seed = strtoul(argv[1], &end, 10);
    if (end == argv[1] || *end != '\0' || errno == ERANGE) {
        fprintf(stderr, "bad SEED: %s\n", argv[1]);
        return 1;
    }

    errno = 0;
    long n = strtol(argv[2], &end, 10);
    if (end == argv[2] || *end != '\0' || errno == ERANGE || n < 1 ||
        n > 1000) {
        fprintf(stderr, "bad N: %s\n", argv[2]);
        return 1;
    }

    int a[1000];
    for (long i = 0; i < n; i++)
        a[i] = (int)i;

    srand((unsigned)seed);
    for (long i = n - 1; i > 0; i--) {
        long j = rand() % (i + 1);
        int t = a[i];
        a[i] = a[j];
        a[j] = t;
    }

    for (long i = 0; i < n; i++)
        printf(i ? " %d" : "%d", a[i]);
    putchar('\n');
    return 0;
}
