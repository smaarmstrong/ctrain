/* Test harness — provides main() and drives the learner's searches. */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

long find_first(const int *a, size_t n, int key);
long find_last(const int *a, size_t n, int key);

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

static unsigned long rng_state = 0xda3e39cb94b95bdbUL;
static unsigned rnd(void)
{
    rng_state = rng_state * 6364136223846793005UL + 1442695040888963407UL;
    return (unsigned)(rng_state >> 33);
}

int main(void)
{
    /* empty */
    CHECK(find_first(NULL, 0, 5) == -1, "find_first on an empty array is -1");
    CHECK(find_last(NULL, 0, 5) == -1, "find_last on an empty array is -1");

    /* single element */
    {
        int a[1] = {4};
        CHECK(find_first(a, 1, 4) == 0 && find_last(a, 1, 4) == 0,
              "single element found at index 0");
        CHECK(find_first(a, 1, 3) == -1 && find_last(a, 1, 5) == -1,
              "single element, absent keys give -1");
    }

    /* the prompt example plus edges and gaps */
    {
        const int a[8] = {1, 2, 2, 2, 3, 5, 5, 9};
        CHECK(find_first(a, 8, 2) == 1, "first of run: {1,2,2,2,3,5,5,9} key 2 -> 1");
        CHECK(find_last(a, 8, 2) == 3, "last of run: key 2 -> 3");
        CHECK(find_first(a, 8, 5) == 5 && find_last(a, 8, 5) == 6,
              "run of two: key 5 -> [5, 6]");
        CHECK(find_first(a, 8, 1) == 0 && find_last(a, 8, 1) == 0,
              "unique key at the very front");
        CHECK(find_first(a, 8, 9) == 7 && find_last(a, 8, 9) == 7,
              "unique key at the very back");
        CHECK(find_first(a, 8, 4) == -1 && find_last(a, 8, 4) == -1,
              "key in a gap between values gives -1");
        CHECK(find_first(a, 8, 0) == -1 && find_last(a, 8, 0) == -1,
              "key below the whole array gives -1");
        CHECK(find_first(a, 8, 10) == -1 && find_last(a, 8, 10) == -1,
              "key above the whole array gives -1");
    }

    /* small all-equal array */
    {
        int a[64];
        for (int i = 0; i < 64; i++)
            a[i] = 7;
        CHECK(find_first(a, 64, 7) == 0 && find_last(a, 64, 7) == 63,
              "all-equal 64: bounds are 0 and 63");
    }

    enum { N = 1 << 20 };
    int *big = malloc((size_t)N * sizeof *big);
    if (!big) {
        fprintf(stderr, "harness: out of memory\n");
        return 2;
    }

    /* big array: even values only, each in a run of four:
     * big[i] = (i / 4) * 2, values 0, 2, ..., 524286 */
    for (long i = 0; i < N; i++)
        big[i] = (int)((i / 4) * 2);
    {
        int ok = 1;
        for (int q = 0; q < 200000 && ok; q++) {
            int key = (int)(rnd() % 524288); /* 0..524287, odd = absent */
            long f = find_first(big, N, key);
            long l = find_last(big, N, key);
            if (key % 2 == 0) {
                long base = (long)(key / 2) * 4;
                if (f != base || l != base + 3)
                    ok = 0;
            } else if (f != -1 || l != -1) {
                ok = 0;
            }
        }
        CHECK(ok, "200000 lookups in 1048576 elements within the time limit");
        CHECK(find_first(big, N, 0) == 0 && find_last(big, N, 0) == 3,
              "big array: lowest value's run is [0, 3]");
        CHECK(find_first(big, N, 524286) == N - 4 &&
              find_last(big, N, 524286) == N - 1,
              "big array: highest value's run ends at n-1");
    }

    /* one giant run: walking out from a middle hit is linear and times out */
    for (long i = 0; i < N; i++)
        big[i] = 42;
    {
        int ok = 1;
        for (int q = 0; q < 100000 && ok; q++) {
            if (find_first(big, N, 42) != 0 || find_last(big, N, 42) != N - 1)
                ok = 0;
        }
        CHECK(ok, "100000 lookups in one giant run within the time limit");
        CHECK(find_first(big, N, 41) == -1 && find_last(big, N, 43) == -1,
              "giant run: neighbouring keys are absent");
    }

    free(big);

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
