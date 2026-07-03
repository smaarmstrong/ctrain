#include <stdio.h>

int main(void)
{
    /* Looks right on a terminal — but redirect both streams to a file
     * and the order falls apart. Why? What is stdout doing differently
     * once it no longer points at a terminal? */
    for (int i = 1; i <= 3; i++) {
        printf("tick %d\n", i);
        fprintf(stderr, "tock %d\n", i);
    }
    return 0;
}
