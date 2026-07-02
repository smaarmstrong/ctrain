/* Test harness — provides main() and calls the learner's functions. */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(int *a, int *b);
void reverse(int *a, size_t n);
int *clone_ints(const int *src, size_t n);

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
    /* swap */
    int x = 3, y = 7;
    swap(&x, &y);
    CHECK(x == 7 && y == 3, "swap exchanges two values");
    swap(&x, &x);
    CHECK(x == 7, "swap(&x, &x) leaves the value unchanged");

    /* reverse — even, odd, single, empty */
    int even[4] = {1, 2, 3, 4};
    reverse(even, 4);
    CHECK(even[0] == 4 && even[1] == 3 && even[2] == 2 && even[3] == 1,
          "reverse of an even-length array");
    int odd[5] = {1, 2, 3, 4, 5};
    reverse(odd, 5);
    CHECK(odd[0] == 5 && odd[2] == 3 && odd[4] == 1,
          "reverse of an odd-length array");
    int one[1] = {42};
    reverse(one, 1);
    CHECK(one[0] == 42, "reverse of a single element");
    reverse(NULL, 0);
    CHECK(1, "reverse(NULL, 0) does not crash");

    /* clone_ints — content, independence, ownership */
    int src[3] = {10, 20, 30};
    int *dup = clone_ints(src, 3);
    CHECK(dup != NULL, "clone_ints returns memory");
    if (dup) {
        CHECK(memcmp(dup, src, sizeof src) == 0, "clone matches the source");
        dup[0] = 99;
        CHECK(src[0] == 10, "clone is independent of the source");
        free(dup); /* under ASan: wrong-size alloc or non-heap pointer dies here */
    } else {
        failures += 2;
    }
    CHECK(clone_ints(src, 0) == NULL, "clone_ints(_, 0) returns NULL");

    /* a larger clone so an off-by-one allocation trips ASan */
    enum { N = 1000 };
    int *big = malloc(N * sizeof *big);
    if (!big)
        return 2;
    for (int i = 0; i < N; i++)
        big[i] = i;
    int *bigdup = clone_ints(big, N);
    CHECK(bigdup && bigdup[N - 1] == N - 1, "clone of 1000 ints, last element intact");
    free(bigdup);
    free(big);

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
