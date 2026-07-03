#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { NAME_LEN = 16, REC_LEN = 20 };

static int usage(void)
{
    fprintf(stderr, "usage: prog write FILE | prog get FILE INDEX\n");
    return 2;
}

static int cannot_open(const char *path)
{
    fprintf(stderr, "record-file: cannot open %s\n", path);
    return 1;
}

static int do_write(const char *path)
{
    FILE *f = fopen(path, "wb");
    if (f == NULL)
        return cannot_open(path);

    char line[256];
    while (fgets(line, sizeof line, stdin) != NULL) {
        char name[NAME_LEN];
        unsigned long qty;
        if (sscanf(line, "%15s %lu", name, &qty) != 2)
            continue; /* input is promised well-formed */

        unsigned char rec[REC_LEN] = {0};
        memcpy(rec, name, strlen(name));
        rec[16] = (unsigned char)(qty & 0xff);
        rec[17] = (unsigned char)((qty >> 8) & 0xff);
        rec[18] = (unsigned char)((qty >> 16) & 0xff);
        rec[19] = (unsigned char)((qty >> 24) & 0xff);
        if (fwrite(rec, 1, REC_LEN, f) != REC_LEN) {
            fclose(f);
            return cannot_open(path);
        }
    }
    fclose(f);
    return 0;
}

static int do_get(const char *path, const char *index_arg)
{
    char *end = NULL;
    long idx = strtol(index_arg, &end, 10);
    if (end == index_arg || *end != '\0' || idx < 0)
        return usage();

    FILE *f = fopen(path, "rb");
    if (f == NULL)
        return cannot_open(path);

    unsigned char rec[REC_LEN];
    if (fseek(f, idx * (long)REC_LEN, SEEK_SET) != 0 ||
        fread(rec, 1, REC_LEN, f) != REC_LEN) {
        fclose(f);
        fprintf(stderr, "record-file: no record %ld\n", idx);
        return 1;
    }
    fclose(f);

    char name[NAME_LEN + 1];
    memcpy(name, rec, NAME_LEN);
    name[NAME_LEN] = '\0';
    uint32_t qty = (uint32_t)rec[16] | ((uint32_t)rec[17] << 8) |
                   ((uint32_t)rec[18] << 16) | ((uint32_t)rec[19] << 24);
    printf("%s %lu\n", name, (unsigned long)qty);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc == 3 && strcmp(argv[1], "write") == 0)
        return do_write(argv[2]);
    if (argc == 4 && strcmp(argv[1], "get") == 0)
        return do_get(argv[2], argv[3]);
    return usage();
}
