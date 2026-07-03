#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    /* TODO:
     *   1. validate: exactly 2 args, both pure base-10 numbers
     *      (strtol/strtoul + end pointer), 1 <= N <= 1000 — else exit 1
     *   2. fill a[0..N-1] with 0..N-1
     *   3. srand(SEED) once, then for i = N-1 down to 1:
     *          j = rand() % (i + 1); swap a[i], a[j];
     *   4. print space-separated on one line
     */
    (void)argc;
    (void)argv;
    printf("TODO\n");
    return 0;
}
