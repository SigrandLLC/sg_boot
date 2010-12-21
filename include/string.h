#ifndef __STRING_H__
#define __STRING_H__

#include <stdlib.h> // size_t

void *memcpy (void *dest, const void *src, size_t n);
void *memset (void *s, int c, size_t n);
size_t strlen(const char *s);
char *strcpy (char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);

#endif

