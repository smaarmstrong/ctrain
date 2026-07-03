#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    /* Happy-path only: no usage check, errors reported on the WRONG
     * stream, always exits 0. Fix the error handling:
     *   - usage errors  -> stderr, exit 2
     *   - open failures -> "head: FILE: <strerror(errno)>" on stderr,
     *                      keep going, exit 1 at the end
     *   - success       -> exit 0
     */
    int count = atoi(argv[1]);
    for (int i = 2; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (f == NULL) {
            printf("could not open %s\n", argv[i]);
            continue;
        }
        char buf[1024];
        int lines = 0;
        while (lines < count && fgets(buf, sizeof buf, f) != NULL) {
            fputs(buf, stdout);
            lines++;
        }
        fclose(f);
    }
    return 0;
}
