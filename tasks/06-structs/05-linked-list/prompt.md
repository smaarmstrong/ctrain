# singly linked list — build, insert, free

Implement the functions below in `main.c`. Do **not** define `main` — the
grader links your file against a test harness that provides it.

Copy this self-referential struct into your file exactly as written (the
harness walks the `next` pointers directly):

```c
#include <stddef.h>

struct node {
    int value;
    struct node *next;
};
```

Every node lives on the heap (`malloc`). The empty list is `NULL`. You
may assume `malloc` never fails in the grader; handling failure is
optional.

```c
/* Build a list containing vals[0..n-1] IN THAT ORDER (vals[0] becomes
 * the head). Return the head, or NULL when n is 0 (vals may then be
 * NULL too). */
struct node *list_from_array(const int *vals, size_t n);

/* head is a list already sorted ascending (possibly NULL). Allocate a
 * node for value and splice it in so the list stays sorted. Duplicates
 * are allowed. Return the (possibly new) head. */
struct node *list_insert_sorted(struct node *head, int value);

/* Number of nodes in the list. 0 for NULL. */
size_t list_length(const struct node *head);

/* Free every node. list_free(NULL) is a no-op. */
void list_free(struct node *head);
```

The harness first checks behaviour (node order, lengths), then reruns
everything under AddressSanitizer: a leaked node, a double free, or a
read of freed memory fails the task even when the values come out right.

Grade with `ctrain check linked-list`.
