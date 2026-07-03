#define _POSIX_C_SOURCE 200809L
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static int by_name(const void *a, const void *b)
{
    const char *const *x = a;
    const char *const *y = b;
    return strcmp(*x, *y);
}

static char *xstrdup(const char *s)
{
    size_t len = strlen(s) + 1;
    char *p = malloc(len);
    if (p)
        memcpy(p, s, len);
    return p;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s DIR\n", argv[0]);
        return 1;
    }

    DIR *d = opendir(argv[1]);
    if (!d) {
        fprintf(stderr, "cannot open directory: %s\n", argv[1]);
        return 1;
    }

    char **names = NULL;
    size_t n = 0, cap = 0;
    struct dirent *e;
    while ((e = readdir(d)) != NULL) {
        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0)
            continue;
        if (n == cap) {
            cap = cap ? cap * 2 : 16;
            char **grown = realloc(names, cap * sizeof *names);
            if (!grown) {
                fprintf(stderr, "out of memory\n");
                closedir(d);
                return 1;
            }
            names = grown;
        }
        names[n] = xstrdup(e->d_name);
        if (!names[n]) {
            fprintf(stderr, "out of memory\n");
            closedir(d);
            return 1;
        }
        n++;
    }
    closedir(d);

    qsort(names, n, sizeof *names, by_name);

    int status = 0;
    for (size_t i = 0; i < n; i++) {
        size_t plen = strlen(argv[1]) + 1 + strlen(names[i]) + 1;
        char *path = malloc(plen);
        if (!path) {
            status = 1;
            break;
        }
        strcpy(path, argv[1]);
        strcat(path, "/");
        strcat(path, names[i]);

        struct stat st;
        if (stat(path, &st) != 0) {
            fprintf(stderr, "cannot stat: %s\n", path);
            free(path);
            status = 1;
            break;
        }
        free(path);

        if (S_ISREG(st.st_mode))
            printf("%lld %s\n", (long long)st.st_size, names[i]);
        else if (S_ISDIR(st.st_mode))
            printf("dir %s\n", names[i]);
        else
            printf("other %s\n", names[i]);
    }

    for (size_t i = 0; i < n; i++)
        free(names[i]);
    free(names);
    return status;
}
