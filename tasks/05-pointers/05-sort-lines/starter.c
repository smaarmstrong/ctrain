#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Plan:
 *   1. read each line into its own malloc'd buffer (grow as needed)
 *   2. keep the buffers in a growing array of char *
 *   3. sort the POINTERS (strcmp order)
 *   4. print, then free every buffer and the array
 */

int main(void)
{
    /* TODO — for now this just echoes the input unsorted. */
    int c;
    while ((c = getchar()) != EOF)
        putchar(c);
    return 0;
}
