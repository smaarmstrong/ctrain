#include <stdio.h>
#include <string.h>

/* Plan:
 *   1. scan argv for -v / -n; the remaining argument is the pattern
 *   2. wrong shape of command line -> usage line on stderr, exit 2
 *   3. read stdin line by line (fgets with a 1024 buffer is enough),
 *      strip the '\n', test with strstr, print (numbered with -n)
 *   4. exit 0 if anything was printed, else 1
 */

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    /* TODO */
    return 1;
}
