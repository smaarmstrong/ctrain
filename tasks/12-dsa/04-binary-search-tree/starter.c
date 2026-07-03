#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct bst BST;

struct bnode {
    int           key;
    struct bnode *left;
    struct bnode *right;
};

struct bst {
    struct bnode *root;
    size_t        count;
};

BST *bst_new(void)
{
    /* TODO: allocate an empty tree. */
    return NULL;
}

void bst_free(BST *t)
{
    /* TODO: free every node (post-order works), then the struct.
     * NULL must be a no-op. */
    (void)t;
}

size_t bst_size(const BST *t)
{
    (void)t;
    return 0; /* TODO */
}

bool bst_insert(BST *t, int key)
{
    /* TODO: walk down; refuse duplicates. */
    (void)t;
    (void)key;
    return false;
}

bool bst_contains(const BST *t, int key)
{
    /* TODO */
    (void)t;
    (void)key;
    return false;
}

bool bst_remove(BST *t, int key)
{
    /* TODO: leaf, one-child, and two-children cases.
     * For two children, replace with the in-order successor (or
     * predecessor) and delete that node instead. */
    (void)t;
    (void)key;
    return false;
}

size_t bst_to_array(const BST *t, int *out, size_t max)
{
    /* TODO: in-order walk, stop after max keys, return how many written. */
    (void)t;
    (void)out;
    (void)max;
    return 0;
}
