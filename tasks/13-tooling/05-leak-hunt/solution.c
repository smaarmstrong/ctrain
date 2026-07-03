#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    char *text;
    struct node *next;
};

int main(void)
{
    struct node *head = NULL;
    char buf[1024];
    size_t count = 0;

    while (fgets(buf, sizeof buf, stdin) != NULL) {
        struct node *n = malloc(sizeof *n);
        if (n == NULL)
            return 1;
        n->text = malloc(strlen(buf) + 1);
        if (n->text == NULL) {
            free(n);
            return 1;
        }
        strcpy(n->text, buf);
        n->next = head;
        head = n;
        count++;
    }

    for (struct node *n = head; n != NULL; n = n->next)
        fputs(n->text, stdout);
    printf("-- %zu lines\n", count);

    while (head != NULL) {
        struct node *next = head->next;
        free(head->text);
        free(head);
        head = next;
    }
    return 0;
}
