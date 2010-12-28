#ifndef __STRING_H__
#define __STRING_H__

#include <stdlib.h> // size_t

void *memcpy  (void *dest, const void *src, size_t n);
void *memset  (void *s, int c, size_t n);
int   memcmp  (const void *s1, const void *s2, size_t n);
int  strncmp  (const char *s1, const char *s2, size_t n);
int   strcmp  (const char *s1, const char *s2);

size_t strlen (const char *s);
char * strcpy (char *dest, const char *src);
char * strncpy(char *dest, const char *src, size_t n);

#endif

