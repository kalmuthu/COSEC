#include <std/string.h>
#include <arch/i386.h>
#include <mm/kheap.h>

inline int strncmp(const char *s1, const char *s2, size_t n) {
    if (s1 == s2) return 0;
    int i = 0;
    while (i++ < n) {
        if ((*s1) != (*s2)) return ((*s2) - (*s1));
        if (0 == (*s1)) return 0;
        ++s1, ++s2;
    }
    return 0;
}

int strcmp(const char *s1, const char *s2) {
    return strncmp(s1, s2, MAX_UINT);
}

int memcmp(const void *s1, const void *s2, size_t n) {
    int i;
    const char *c1 = s1;
    const char *c2 = s2;
    for (i = 0; i < (int)n; ++i) {
        char diff = *c1 - *c2;
        if (diff)
            return diff;
        ++c2; ++c1;
    }
    return 0;
}

inline char *strndup(const char *s, size_t n) {
    if (s == null) return null;
    size_t len = strlen(s) + 1;
    if (len > n) len = n;
    char *d = (char *) kmalloc(len);
    return strncpy(d, s, n);
}

char *strdup(const char *s) {
    return strndup(s, MAX_UINT);
}

char *strcpy(char *dest, const char *src) {
    if ((src == null) || (dest == null))
        return null;
    char *d = dest;
    while ((*d++ = *src++));
    return dest;
}

char *strncpy(char *dest, const char *src, size_t n) {
    size_t i = 0;
    char *d = dest;
    while ((*src) && (i++ < n))
        *d++ = *src++;
    /*while (i++ < n)  *d++ = '\0'; // Sorry: POSIX requires this */
    return dest;
}

size_t strlen(const char *s) {
    const char *c = s;
    while (*c) ++c;
    return c - s;
}

size_t strnlen(const char *s, size_t maxlen) {
    const char *c = s;
    size_t i = 0;
    while ((i++ < maxlen) && *c) ++c;
    return c - s;
}

void* memcpy(void *dest, const void *src, size_t size) {
    //arch_memcpy(dest, src, size);
    size_t i = 0;
    uint8_t *d = dest;
    const uint8_t *s = src;
    while (i++ < size)
        *d++ = *s++;
    return dest;
}

void *memset(void *s, int c, size_t n) {
    char *p = (char *)s;
    unsigned i;
    for (i = 0; i < n; ++i)
        p[i] = c;
    return s;
}

char *strnchr(char *s, size_t n, char c) {
    char *cur = (char *)s;
    while (*cur && ((cur - s) < (int)n)) {
        if (c == *cur)
            return cur;
        ++cur;
    }
    return null;
}

char *strnrchr(char *s, size_t n, char c) {
    char *cur = (char *)s;
    char *last = null;
    while (*cur && ((cur - s) < (int)n)) {
        if (c == *cur)
            last = cur;
        ++cur;
    }
    return last;
}

char *strchr(char *s, char c) {
    return strnchr(s, MAX_INT, c);
}

char *strrchr(char *s, char c) {
    return strnrchr(s, MAX_INT, c);
}
