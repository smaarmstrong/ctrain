/* Test harness — provides main() and calls the learner's functions. */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct node {
    int value;
    struct node *next;
};

struct node *list_from_array(const int *vals, size_t n);
struct node *list_insert_sorted(struct node *head, int value);
size_t list_length(const struct node *head);
void list_free(struct node *head);

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

/* 1 if the list holds exactly want[0..n-1] in order and ends in NULL. */
static int list_is(const struct node *head, const int *want, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        if (!head || head->value != want[i])
            return 0;
        head = head->next;
    }
    return head == NULL;
}

int main(void)
{
    CHECK(list_from_array(NULL, 0) == NULL, "an empty array builds a NULL list");
    CHECK(list_length(NULL) == 0, "length of NULL is 0");

    int vals[3] = {3, 1, 2};
    struct node *l = list_from_array(vals, 3);
    CHECK(list_is(l, vals, 3), "list_from_array preserves order 3, 1, 2");
    CHECK(list_length(l) == 3, "length of a 3-node list is 3");
    list_free(l);

    struct node *s = list_insert_sorted(NULL, 5);
    int just5[1] = {5};
    CHECK(list_is(s, just5, 1), "insert into an empty list makes a 1-node list");

    int base[3] = {2, 4, 6};
    struct node *sorted = list_from_array(base, 3);
    sorted = list_insert_sorted(sorted, 1); /* new head */
    sorted = list_insert_sorted(sorted, 5); /* middle */
    sorted = list_insert_sorted(sorted, 9); /* tail */
    sorted = list_insert_sorted(sorted, 4); /* duplicate */
    int want[7] = {1, 2, 4, 4, 5, 6, 9};
    CHECK(list_is(sorted, want, 7), "inserts at head, middle, tail and duplicate stay sorted");
    CHECK(list_length(sorted) == 7, "length after four inserts is 7");
    list_free(sorted);
    list_free(s);
    list_free(NULL);
    CHECK(1, "list_free(NULL) does not crash");

    /* scale: leaks of even one node per operation show up under ASan */
    enum { N = 500 };
    int *big = malloc(N * sizeof *big);
    if (!big)
        return 2;
    for (int i = 0; i < N; i++)
        big[i] = i;
    struct node *bl = list_from_array(big, N);
    CHECK(list_length(bl) == N, "a 500-node list has length 500");
    CHECK(list_is(bl, big, N), "all 500 values in order");
    for (int i = 0; i < 50; i++)
        bl = list_insert_sorted(bl, i * 10);
    CHECK(list_length(bl) == N + 50, "50 sorted inserts grow the list to 550");
    list_free(bl);
    free(big);

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
