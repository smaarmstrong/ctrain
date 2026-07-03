#include <stdio.h>

int main(void)
{
    for (int i = 1; i <= 3; i++) {
        printf("tick %d\n", i);
        fflush(stdout); /* push it past the full buffering NOW */
        fprintf(stderr, "tock %d\n", i);
    }
    return 0;
}
