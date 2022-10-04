#ifndef LIBC_H__
#define LIBC_H__

#ifndef NO_ALIAS
#include "types.h"
#else
#include <stddef.h>
#endif

#include <stdarg.h>

#ifndef NO_ALIAS
#define memset mmemset
#define memcpy mmemcpy
#define strlen mstrlen
#define vsnprintf mvsnprintf
#define strncmp mstrncmp
#endif

void *mmemset(void *_s, int c, size_t n);
void *mmemcpy(void *dest, const void *src, size_t n);
size_t mstrlen(const char *s);
int mvsnprintf(char *str, size_t size, const char *fmt, va_list ap);
int mstrncmp(const char *s1, const char *s2, size_t n);

#endif/*LIBC_H__*/
