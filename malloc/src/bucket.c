#include <err.h>
#include <stddef.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "libs.h"

void *page_begin(void *ptr, size_t page_size)
{
    unsigned char *p = ptr;
    size_t masque = (size_t)p & (page_size - 1);
    unsigned char *begin = p - masque;
    return begin;
}

size_t align(size_t size)
{
    size_t alignment = sizeof(long double);
    size_t remainder = size % alignment;
    if (remainder != 0)
        size += alignment - remainder;

    size_t temp;
    if (__builtin_mul_overflow(size, alignment - remainder, &temp))
        return 0;

    return size;
}
