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

    /* one draw per statement: the order is now part of the program */
    for (int i = 0; i < 2; i++) {
        int left = next_ticket();
        int right = next_ticket();
        printf("pair: left=%d right=%d\n", left, right);
    }

    int slots[16] = {0};
    int slot = next_ticket() % 16;
    int value = next_ticket();
    slots[slot] = value;
    for (int i = 0; i < 16; i++)
        if (slots[i] != 0)
            printf("slot %d stores %d\n", i, slots[i]);

    printf("total issued: %d\n", counter - 1);
    return 0;
}
