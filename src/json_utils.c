#include "json_utils.h"
#include <string.h>

/* Allocates memory of the specified size. */
void *json_alloc(size_t size)
{
    return malloc(size);
}

/* Reallocates memory to a new size. */
void *json_realloc(void *ptr, size_t size)
{
    return realloc(ptr, size);
}

/* Frees the allocated memory. */
void json_free(void *ptr)
{
    free(ptr);
}

/* Duplicates a null-terminated string. */
char *json_strdup(const char *s)
{
    size_t len = strlen(s);
    char *dup = json_alloc(len + 1);
    if (dup)
    {
        memcpy(dup, s, len + 1);
    }
    return dup;
}

/* Duplicates a range of characters from a string. */
char *json_strdup_range(const char *s, size_t len)
{
    char *dup = json_alloc(len + 1);
    if (dup)
    {
        memcpy(dup, s, len);
        dup[len] = '\0';
    }
    return dup;
}

/* Checks if a character is considered whitespace in JSON. */
int json_is_whitespace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}
