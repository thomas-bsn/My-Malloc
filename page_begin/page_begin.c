#include "page_begin.h"

#include <stddef.h>

void *page_begin(void *ptr, size_t page_size)
{
    unsigned char *p = ptr;
    size_t masque = (size_t)p & (page_size - 1);
    unsigned char *begin = p - masque;
    return begin;
}
