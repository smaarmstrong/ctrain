/* Test harness — provides main() and exercises the learner's stack.
 * Per the contract, the harness frees every string it pops. */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **items;
    size_t count;
    size_t cap;
} sstack;

void sstack_init(sstack *s);
int sstack_push(sstack *s, const char *str);
char *sstack_pop(sstack *s);
void sstack_destroy(sstack *s);

static int failures;

#define CHECK(cond, msg)                                        \
    do {                                                        \
        if (cond) {                                             \
            printf("  ok    %s\n", msg);                        \
        } else {                                                \
            printf("  FAIL  %s\n", msg);                        \
            failures++;                                         \
        }                                                       \
    } while (0)

int main(void)
{
    sstack st;
    sstack_init(&st);
    CHECK(st.items == NULL && st.count == 0 && st.cap == 0,
          "init produces the empty state");
    CHECK(sstack_pop(&st) == NULL, "pop on an empty stack returns NULL");

    CHECK(sstack_push(&st, "alpha") == 0, "push alpha");
    CHECK(sstack_push(&st, "beta") == 0, "push beta");
    CHECK(sstack_push(&st, "gamma") == 0, "push gamma");
    CHECK(st.count == 3, "count is 3 after three pushes");

    /* the stack must store copies, not the caller's buffer */
    {
        char scratch[16];
        strcpy(scratch, "delta");
        sstack_push(&st, scratch);
        strcpy(scratch, "MANGLED");
        char *p = sstack_pop(&st);
        CHECK(p != NULL && strcmp(p, "delta") == 0,
              "pushed string is copied, not aliased");
        free(p); /* caller owns popped strings */
    }

    /* LIFO order, contents intact, caller frees */
    {
        char *p = sstack_pop(&st);
        CHECK(p != NULL && strcmp(p, "gamma") == 0,
              "pop returns the top string with its contents intact");
        free(p);
        p = sstack_pop(&st);
        CHECK(p != NULL && strcmp(p, "beta") == 0, "pop returns beta next");
        free(p);
        CHECK(st.count == 1, "one string left on the stack");
    }

    /* growth past the initial capacity */
    for (int i = 0; i < 20; i++) {
        char buf[16];
        snprintf(buf, sizeof buf, "item-%d", i);
        if (sstack_push(&st, buf) != 0)
            return 2;
    }
    CHECK(st.count == 21, "count is 21 after growth");
    {
        char *p = sstack_pop(&st);
        CHECK(p != NULL && strcmp(p, "item-19") == 0,
              "pop after growth returns the newest item");
        free(p);
    }

    /* destroy frees the leftovers and resets the struct */
    sstack_destroy(&st);
    CHECK(st.items == NULL && st.count == 0 && st.cap == 0,
          "destroy resets to the empty state");
    sstack_destroy(&st); /* must be a safe no-op */
    CHECK(1, "destroying an already-destroyed stack is safe");

    /* the stack is reusable after destroy */
    CHECK(sstack_push(&st, "again") == 0, "push works after destroy");
    {
        char *p = sstack_pop(&st);
        CHECK(p != NULL && strcmp(p, "again") == 0, "reused stack pops correctly");
        free(p);
    }
    sstack_destroy(&st);

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
