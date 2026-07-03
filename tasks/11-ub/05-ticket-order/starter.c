#include <stdio.h>

static int counter = 1;

static int next_ticket(void)
{
    return counter++;
}

static void serve(const char *name)
{
    printf("%s: %d\n", name, next_ticket());
}

int main(void)
{
    serve("alice");
    serve("bob");
    serve("carol");

    /* two pair draws: left is meant to be drawn before right */
    for (int i = 0; i < 2; i++)
        printf("pair: left=%d right=%d\n", next_ticket(), next_ticket());

    /* the first draw picks the slot, the second the value stored there */
    int slots[16] = {0};
    slots[next_ticket() % 16] = next_ticket();
    for (int i = 0; i < 16; i++)
        if (slots[i] != 0)
            printf("slot %d stores %d\n", i, slots[i]);

    printf("total issued: %d\n", counter - 1);
    return 0;
}
