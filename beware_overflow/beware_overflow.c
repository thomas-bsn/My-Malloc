#include "beware_overflow.h"

#include <stddef.h>

void *beware_overflow(void *ptr, size_t nmemb, size_t size)
{
    size_t temp;
    if (__builtin_mul_overflow(nmemb, size, &temp))
        return NULL;

    char *p = ptr;
    return p + temp;
}
