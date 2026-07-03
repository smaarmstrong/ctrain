#include <stddef.h>
#include <stdint.h>

#define POOL_ALIGN _Alignof(max_align_t)

typedef struct block {
    size_t size;        /* payload bytes in this block            */
    int used;           /* 0 = free, 1 = handed out               */
    struct block *next; /* next block in address order, or NULL   */
} block;

static block *head;

static size_t align_up(size_t n)
{
    return (n + POOL_ALIGN - 1) / POOL_ALIGN * POOL_ALIGN;
}

static size_t header_size(void)
{
    return align_up(sizeof(block));
}

void pool_init(void *mem, size_t size)
{
    uintptr_t start = (uintptr_t)mem;
    uintptr_t aligned = (start + POOL_ALIGN - 1) / POOL_ALIGN * POOL_ALIGN;
    size_t lost = (size_t)(aligned - start);

    head = NULL;
    if (size < lost || size - lost < header_size() + POOL_ALIGN)
        return;

    head = (block *)aligned;
    head->size = size - lost - header_size();
    head->used = 0;
    head->next = NULL;
}

void *pool_alloc(size_t n)
{
    if (n == 0)
        return NULL;
    size_t need = align_up(n);

    for (block *b = head; b; b = b->next) {
        if (b->used || b->size < need)
            continue;
        /* split when the remainder can hold a header plus something */
        if (b->size >= need + header_size() + POOL_ALIGN) {
            block *rest = (block *)((char *)b + header_size() + need);
            rest->size = b->size - need - header_size();
            rest->used = 0;
            rest->next = b->next;
            b->next = rest;
            b->size = need;
        }
        b->used = 1;
        return (char *)b + header_size();
    }
    return NULL;
}

void pool_free(void *p)
{
    if (!p)
        return;
    block *b = (block *)((char *)p - header_size());
    b->used = 0;

    /* merge every run of adjacent free blocks (list is address-ordered) */
    for (block *cur = head; cur;) {
        block *nx = cur->next;
        if (!cur->used && nx && !nx->used) {
            cur->size += header_size() + nx->size;
            cur->next = nx->next;
        } else {
            cur = nx;
        }
    }
}
