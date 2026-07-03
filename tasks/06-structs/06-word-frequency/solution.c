#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tnode {
    char *word;
    int count;
    struct tnode *left;
    struct tnode *right;
};

static char *copy_word(const char *s)
{
    char *p = malloc(strlen(s) + 1);
    if (!p) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }
    strcpy(p, s);
    return p;
}

static struct tnode *insert(struct tnode *root, const char *word)
{
    if (!root) {
        struct tnode *n = malloc(sizeof *n);
        if (!n) {
            fprintf(stderr, "out of memory\n");
            exit(1);
        }
        n->word = copy_word(word);
        n->count = 1;
        n->left = n->right = NULL;
        return n;
    }
    int cmp = strcmp(word, root->word);
    if (cmp == 0)
        root->count++;
    else if (cmp < 0)
        root->left = insert(root->left, word);
    else
        root->right = insert(root->right, word);
    return root;
}

static void print_inorder(const struct tnode *root)
{
    if (!root)
        return;
    print_inorder(root->left);
    printf("%s %d\n", root->word, root->count);
    print_inorder(root->right);
}

static void free_tree(struct tnode *root)
{
    if (!root)
        return;
    free_tree(root->left);
    free_tree(root->right);
    free(root->word);
    free(root);
}

int main(void)
{
    struct tnode *root = NULL;
    char buf[64];

    while (scanf("%63s", buf) == 1)
        root = insert(root, buf);

    print_inorder(root);
    free_tree(root);
    return 0;
}
