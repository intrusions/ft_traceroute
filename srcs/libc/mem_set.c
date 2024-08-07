#include <stddef.h>

void *mem_set(void *s, int c, size_t n)
{
    unsigned char *ptr = (unsigned char *)s;
    size_t i;

    for (i = 0; i < n; i++)
        ptr[i] = (unsigned char)c;

    return s;
}
