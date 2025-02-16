#ifndef LIBS_H
#define LIBS_H

#include <err.h>
#include <stddef.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

struct blk_meta
{
    size_t size;
    void *data;
    struct blk_meta *next;
};

void *page_begin(void *ptr, size_t page_size);
size_t align(size_t size);

extern struct blk_meta *free_list;

#endif /* !LIBS_H */