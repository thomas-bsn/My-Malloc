#include "allocator.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

struct blk_allocator *blka_new(void)
{
    struct blk_allocator *ptr = malloc(sizeof(struct blk_allocator));
    if (!ptr)
        return NULL;
    ptr->meta = NULL;
    return ptr;
}

struct blk_meta *blka_alloc(struct blk_allocator *blka, size_t size)
{
    size_t size_page = sysconf(_SC_PAGE_SIZE);
    size_t size_meta = size_page - sizeof(struct blk_meta);
    size_t size_total = size_page;
    while (size > size_meta)
    {
        size_meta += size_page;
        size_total += size_page;
    }
    void *ptr = mmap(NULL, size_total, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED)
        return NULL;
    struct blk_meta *blk = ptr;
    blk->size = size_meta;
    blk->next = blka->meta;
    blka->meta = blk;
    return blk;
}

void blka_free(struct blk_meta *blk)
{
    size_t size_page = blk->size + sizeof(struct blk_meta);
    munmap(blk, size_page);
}

void blka_pop(struct blk_allocator *blka)
{
    if (blka && blka->meta)
    {
        struct blk_meta *blk = blka->meta;
        blka->meta = blk->next;
        blka_free(blk);
    }
}

void blka_delete(struct blk_allocator *blka)
{
    while (blka->meta)
        blka_pop(blka);
    free(blka);
}
