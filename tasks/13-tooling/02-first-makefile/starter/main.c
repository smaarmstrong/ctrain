/* calc — two-module demo program. Finished and correct: do not edit.
 * Your job is the Makefile. See prompt.md.
 */
#include <stdio.h>
#include <stdlib.h>

#include "ops.h"

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "usage: calc A B\n");
        return 1;
    }
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    printf("%d + %d = %d\n", a, b, add(a, b));
    printf("%d * %d = %d\n", a, b, mul(a, b));
    return 0;
}
