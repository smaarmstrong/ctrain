#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Record layout: 20 bytes.
 *   bytes 0..15  name, '\0'-padded
 *   bytes 16..19 quantity, unsigned 32-bit LITTLE-ENDIAN
 *
 * This starter writes TEXT lines instead of binary records, reads the
 * whole file instead of seeking, and knows nothing about errors or
 * exit statuses. Replace the internals with fwrite/fseek/fread. */

int main(int argc, char *argv[])
{
    if (argc >= 3 && strcmp(argv[1], "write") == 0) {
        FILE *f = fopen(argv[2], "w");
        if (f == NULL)
            return 1;
        char line[256];
        while (fgets(line, sizeof line, stdin) != NULL)
            fputs(line, f);
        fclose(f);
        return 0;
    }
    if (argc >= 4 && strcmp(argv[1], "get") == 0) {
        FILE *f = fopen(argv[2], "r");
        if (f == NULL)
            return 1;
        long want = atol(argv[3]);
        char line[256];
        long i = 0;
        while (fgets(line, sizeof line, f) != NULL) {
            if (i++ == want)
                fputs(line, stdout);
        }
        fclose(f);
        return 0;
    }
    return 0;
}
