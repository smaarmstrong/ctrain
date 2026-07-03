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
    BST *t = malloc(sizeof *t);
    if (!t)
        return NULL;
    t->root = NULL;
    t->count = 0;
    return t;
}

static void free_subtree(struct bnode *n)
{
    if (!n)
        return;
    free_subtree(n->left);
    free_subtree(n->right);
    free(n);
}

void bst_free(BST *t)
{
    if (!t)
        return;
    free_subtree(t->root);
    free(t);
}

size_t bst_size(const BST *t)
{
    return t->count;
}

bool bst_insert(BST *t, int key)
{
    struct bnode **link = &t->root;
    while (*link) {
        if (key == (*link)->key)
            return false;
        link = key < (*link)->key ? &(*link)->left : &(*link)->right;
    }
    struct bnode *n = malloc(sizeof *n);
    if (!n)
        return false;
    n->key = key;
    n->left = n->right = NULL;
    *link = n;
    t->count++;
    return true;
}

bool bst_contains(const BST *t, int key)
{
    const struct bnode *n = t->root;
    while (n) {
        if (key == n->key)
            return true;
        n = key < n->key ? n->left : n->right;
    }
    return false;
}

bool bst_remove(BST *t, int key)
{
    struct bnode **link = &t->root;
    while (*link && (*link)->key != key)
        link = key < (*link)->key ? &(*link)->left : &(*link)->right;
    struct bnode *n = *link;
    if (!n)
        return false;

    if (n->left && n->right) {
        /* two children: splice in the in-order successor's key */
        struct bnode **slink = &n->right;
        while ((*slink)->left)
            slink = &(*slink)->left;
        struct bnode *succ = *slink;
        n->key = succ->key;
        *slink = succ->right;
        free(succ);
    } else {
        *link = n->left ? n->left : n->right;
        free(n);
    }
    t->count--;
    return true;
}

static void inorder(const struct bnode *n, int *out, size_t max, size_t *written)
{
    if (!n || *written >= max)
        return;
    inorder(n->left, out, max, written);
    if (*written < max)
        out[(*written)++] = n->key;
    inorder(n->right, out, max, written);
}

size_t bst_to_array(const BST *t, int *out, size_t max)
{
    size_t written = 0;
    inorder(t->root, out, max, &written);
    return written;
}
