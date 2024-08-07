char *str_cpy(char *dest, const char *src)
{
    char *ptr = dest;

    while (*src)
        *ptr++ = *src++;
    *ptr = 0;

    return dest;
}
