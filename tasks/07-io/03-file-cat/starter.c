#include <stdio.h>

int main(int argc, char *argv[])
{
    /* Handles the no-argument case only. TODO: open each named file
     * with fopen, copy it with fgets/fputs, close it with fclose, and
     * report unopenable files on stderr with exit status 1. */
    (void)argv;
    if (argc == 1) {
        char buf[512];
        while (fgets(buf, sizeof buf, stdin) != NULL)
            fputs(buf, stdout);
    }
    return 0;
}
