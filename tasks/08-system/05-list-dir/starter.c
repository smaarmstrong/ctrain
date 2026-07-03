#define _POSIX_C_SOURCE 200809L
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/* TODO:
 *   1. opendir(argv[1]); collect every name except "." and ".."
 *      (copy the strings — the dirent is reused by readdir)
 *   2. qsort the names with a strcmp comparator
 *   3. stat "DIR/name" for each; print "SIZE name", "dir name" or
 *      "other name"
 *   4. free everything; errors go to stderr with a non-zero exit
 */

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s DIR\n", argv[0]);
        return 1;
    }
    printf("TODO %s\n", argv[1]);
    return 0;
}
