/* Test harness — provides main() and calls the learner's functions. */
#include <stddef.h>
#include <stdio.h>

typedef struct {
    int *data;
    size_t len;
    size_t cap;
} intvec;

void vec_init(intvec *v);
int vec_push(intvec *v, int x);
int vec_pop(intvec *v, int *out);
void vec_free(intvec *v);

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
    intvec v;
    vec_init(&v);
    CHECK(v.data == NULL && v.len == 0 && v.cap == 0,
          "init: data NULL, len 0, cap 0");

    /* pop on empty leaves *out alone */
    int out = 123;
    CHECK(vec_pop(&v, &out) == -1 && out == 123,
          "pop on an empty vector fails and leaves *out untouched");

    /* first push allocates exactly 8 */
    CHECK(vec_push(&v, 10) == 0, "first push succeeds");
    CHECK(v.len == 1 && v.cap == 8 && v.data != NULL,
          "first allocation: cap == 8");
    int *after_first = v.data;

    /* pushes 2..8 must not move or regrow the buffer */
    for (int i = 2; i <= 8; i++)
        vec_push(&v, 10 * i);
    CHECK(v.len == 8 && v.cap == 8, "8 elements fit without regrowing");
    CHECK(v.data == after_first, "no reallocation while there is room");

    /* 9th push doubles to 16 */
    CHECK(vec_push(&v, 90) == 0 && v.len == 9 && v.cap == 16,
          "9th push doubles cap to 16");
    CHECK(v.data[0] == 10 && v.data[7] == 80 && v.data[8] == 90,
          "old elements survive the realloc");

    /* keep pushing: cap walks 8,16,32,... exactly */
    int cap_ok = 1;
    for (int i = 10; i <= 20000; i++) {
        if (vec_push(&v, 10 * i) != 0) { cap_ok = 0; break; }
        size_t c = v.cap;
        if (c != 8 && c != 16 && c != 32 && c != 64 && c != 128 &&
            c != 256 && c != 512 && c != 1024 && c != 2048 &&
            c != 4096 && c != 8192 && c != 16384 && c != 32768) {
            cap_ok = 0;
            break;
        }
    }
    CHECK(cap_ok, "cap only ever takes values 8, 16, 32, ... while growing");
    CHECK(v.len == 20000 && v.cap == 32768, "20000 pushes: len 20000, cap 32768");

    /* spot-check contents, then pop everything back in LIFO order */
    CHECK(v.data[12345] == 10 * 12346, "contents intact deep into the buffer");
    int lifo_ok = 1;
    for (int i = 20000; i >= 1; i--) {
        if (vec_pop(&v, &out) != 0 || out != 10 * i) { lifo_ok = 0; break; }
    }
    CHECK(lifo_ok, "20000 pops return everything in LIFO order");
    CHECK(v.len == 0 && v.cap == 32768, "popping empties len but never shrinks cap");
    CHECK(vec_pop(&v, &out) == -1, "pop fails again once empty");

    /* free resets; the vector is reusable */
    vec_free(&v);
    CHECK(v.data == NULL && v.len == 0 && v.cap == 0,
          "free resets to the fresh state");
    CHECK(vec_push(&v, 7) == 0 && v.len == 1 && v.cap == 8 && v.data[0] == 7,
          "vector is reusable after free");
    vec_free(&v);

    /* freeing a fresh vector is a no-op */
    intvec w;
    vec_init(&w);
    vec_free(&w);
    CHECK(w.data == NULL && w.cap == 0, "freeing a never-used vector is safe");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
