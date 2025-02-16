#include "alignment.h"

#include <stddef.h>

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
