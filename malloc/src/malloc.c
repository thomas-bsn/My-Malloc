#include <err.h>
#include <stddef.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "libs.h"
#define _GNU_SOURCE
#define PAGE_SIZE sysconf(_SC_PAGESIZE)

struct blk_meta *block = NULL;

__attribute__((visibility("default"))) void *malloc(size_t size)
{
    struct blk_meta *current;

    if (size == 0)
        return NULL;

    if (block == NULL)
    {
        block = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (block == MAP_FAILED)
            return NULL;
        block->data =
            block + 1; //(void *)((char *)block + sizeof(struct blk_meta));
        block->size = size;
        block->next = NULL;
        return block->data;
    }
    else
    {
        current = block;

        while (current->next != NULL)
            current = current->next;

        struct blk_meta *tmp = page_begin(block, PAGE_SIZE);

        if ((tmp + PAGE_SIZE + current->size + size)
            < tmp + PAGE_SIZE)
        {
            struct blk_meta *new_block =
                (struct blk_meta *)((char *)current + current->size
                                    + sizeof(struct blk_meta));
            new_block->data = new_block + 1; // (void *)(new_block + 1);
            new_block->size = size;
            new_block->next = NULL;
            current->next = new_block;
            return new_block->data;
        }
    }
    struct blk_meta *new_page = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE,
                                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (new_page == MAP_FAILED)
        return NULL;
    new_page->data = new_page + 1; // (void *)(new_page + 1);
    new_page->size = size;
    new_page->next = NULL;
    current->next = new_page;
    return new_page->data;
}

__attribute__((visibility("default"))) void free(void *ptr)
{
    if (ptr == NULL)
        return;

    struct blk_meta *current = block;
    struct blk_meta *previous = NULL;

    while (current != NULL && current->data != ptr)
    {
        previous = current;
        current = current->next;
    }

    if (current == NULL)
        return;

    if (previous == NULL)
        block = current->next;
    else
        previous->next = current->next;

    munmap(current, PAGE_SIZE);
}

__attribute__((visibility("default"))) void *realloc(void *ptr, size_t size)
{
    if (ptr == NULL)
        return malloc(size);

    if (size == 0)
    {
        free(ptr);
        return NULL;
    }

    struct blk_meta *block = (struct blk_meta *)ptr - 1;

    if (size <= block->size)
        return ptr;

    struct blk_meta *current = block;
    while (current->next != NULL)
        current = current->next;

    struct blk_meta *tmp = page_begin(current, PAGE_SIZE);
    if ((tmp + PAGE_SIZE + current->size + size) < tmp + PAGE_SIZE)
    {
        block->size = size;
        return block->data;
    }
    void *new_ptr = malloc(size);
    for (size_t i = 0; i < block->size; i++)
        *((char *)new_ptr + i) = *((char *)block->data + i);
    free(ptr);

    return new_ptr;
}

__attribute__((visibility("default"))) void *calloc(size_t nmemb, size_t size)
{
    size_t temp;
    if (__builtin_mul_overflow(nmemb, size, &temp))
        return NULL;
    void *ptr = malloc(temp);
    if (ptr == NULL)
        return NULL;
    char *p = ptr;
    for (size_t i = 0; i < temp; i++)
        p[i] = 0;
    return ptr;
}
