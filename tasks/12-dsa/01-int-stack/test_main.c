/* Test harness — provides main() and drives the learner's stack. */
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define STACK_CAP 16

typedef struct {
    int    data[STACK_CAP];
    size_t len;
} IntStack;

void   stack_init(IntStack *s);
size_t stack_size(const IntStack *s);
bool   stack_push(IntStack *s, int v);
bool   stack_pop(IntStack *s, int *out);
bool   stack_peek(const IntStack *s, int *out);

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
    IntStack s;
    stack_init(&s);
    CHECK(stack_size(&s) == 0, "fresh stack is empty");

    int out = 42;
    CHECK(!stack_pop(&s, &out), "pop on an empty stack returns false");
    CHECK(out == 42, "a failed pop leaves *out untouched");
    CHECK(!stack_peek(&s, &out), "peek on an empty stack returns false");
    CHECK(out == 42, "a failed peek leaves *out untouched");

    CHECK(stack_push(&s, 1) && stack_push(&s, 2) && stack_push(&s, 3),
          "three pushes succeed");
    CHECK(stack_size(&s) == 3, "size is 3 after three pushes");
    CHECK(stack_peek(&s, &out) && out == 3, "peek sees the newest value");
    CHECK(stack_size(&s) == 3, "peek does not remove");

    CHECK(stack_pop(&s, &out) && out == 3, "first pop returns 3");
    CHECK(stack_pop(&s, &out) && out == 2, "second pop returns 2");
    CHECK(stack_pop(&s, &out) && out == 1, "third pop returns 1 (LIFO)");
    CHECK(stack_size(&s) == 0, "stack drains back to empty");

    /* fill to capacity, then overfill */
    bool all = true;
    for (int i = 1; i <= STACK_CAP; i++)
        all = all && stack_push(&s, i * 10);
    CHECK(all, "pushes up to STACK_CAP all succeed");
    CHECK(stack_size(&s) == STACK_CAP, "size equals STACK_CAP when full");
    CHECK(!stack_push(&s, 999), "push on a full stack returns false");
    CHECK(stack_size(&s) == STACK_CAP, "a rejected push leaves size unchanged");
    CHECK(stack_peek(&s, &out) && out == STACK_CAP * 10,
          "a rejected push leaves the top unchanged");

    bool order = true;
    for (int i = STACK_CAP; i >= 1; i--) {
        if (!stack_pop(&s, &out) || out != i * 10) {
            order = false;
            break;
        }
    }
    CHECK(order, "a full stack drains in exact reverse push order");
    CHECK(stack_size(&s) == 0, "empty again after draining");

    /* interleaved pushes and pops */
    stack_init(&s);
    stack_push(&s, 1);
    stack_push(&s, 2);
    CHECK(stack_pop(&s, &out) && out == 2, "interleaved: newest of two pops first");
    stack_push(&s, 3);
    CHECK(stack_pop(&s, &out) && out == 3, "interleaved: a fresh push pops before older values");
    CHECK(stack_pop(&s, &out) && out == 1, "interleaved: the oldest value pops last");

    /* re-init resets a used stack */
    stack_push(&s, 7);
    stack_init(&s);
    CHECK(stack_size(&s) == 0, "stack_init resets a used stack");
    CHECK(!stack_pop(&s, &out), "nothing to pop after a reset");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
