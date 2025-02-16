#include "recycler.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct recycler *recycler_create(size_t block_size, size_t total_size)
{
    if (block_size == 0 || total_size == 0 || (block_size % sizeof(size_t) != 0)
        || (total_size % block_size) != 0)
        return NULL;

    struct recycler *r = malloc(sizeof(struct recycler));
    if (r == NULL)
        return NULL;

    r->chunk = malloc(total_size);
    if (!(r->chunk))
    {
        free(r);
        return NULL;
    }

    r->block_size = block_size;
    r->capacity = total_size / block_size;
    r->free = r->chunk;

    struct free_list *tmp = r->chunk;

    for (size_t i = 0; i < r->capacity - 1; i++)
    {
        // tmp->next = tmp + r->block_size;
        tmp->next = (struct free_list *)((char *)tmp + block_size);
        tmp = tmp->next;
    }
    tmp->next = NULL;
    return r;
}

void recycler_destroy(struct recycler *r)
{
    if (r == NULL)
        return;

    if (r->chunk)
        free(r->chunk);
    free(r);
}

void *recycler_allocate(struct recycler *r)
{
    if (r == NULL || r->free == NULL)
        return NULL;

    struct free_list *ptr = r->free;
    r->free = ((struct free_list *)r->free)->next;
    return ptr;
}

void recycler_free(struct recycler *r, void *block)
{
    if (r == NULL || block == NULL)
        return;
    else
    {
        struct free_list *ptr = block;
        ptr->next = r->free;
        r->free = block;
    }
}