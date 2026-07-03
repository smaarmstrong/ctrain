#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tnode {
    char *word;
    int count;
    struct tnode *left;
    struct tnode *right;
};

/* TODO: insert(root, word), print_inorder(root), free_tree(root). */

int main(void)
{
    struct tnode *root = NULL;
    char buf[64];

    while (scanf("%63s", buf) == 1) {
        /* TODO: insert buf into the tree (or bump its count). */
    }

    /* TODO: in-order print "word count", then free everything. */
    (void)root;
    return 0;
}
