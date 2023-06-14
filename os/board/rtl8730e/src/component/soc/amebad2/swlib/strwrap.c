#include <section_config.h>
#include <basic_types.h>

#if defined ( __GNUC__ )
#include <_ansi.h>
#endif
#include <string.h>

#include <strproc.h>
#include <memproc.h>

//wrap
LIBC_ROM_TEXT_SECTION
void *__wrap_memset(void *s, int c, size_t n)
{
	return _memset(s, c, n);
}

LIBC_ROM_TEXT_SECTION
void *__wrap_memcpy(void *s1, const void *s2, size_t n)
{
	return _memcpy(s1, s2, n);
}

LIBC_ROM_TEXT_SECTION
int __wrap_memcmp(const void *av, const void *bv, size_t len)
{
	return _memcmp(av, bv, len);
}

LIBC_ROM_TEXT_SECTION
void *__wrap_memchr(const void *src_void, int c, size_t length)
{
	return _memchr(src_void, c, length);
}

LIBC_ROM_TEXT_SECTION
void *__wrap_memmove(void *dst_void, const void *src_void, size_t length)
{
	return _memmove(dst_void,  src_void, length);
}

LIBC_ROM_TEXT_SECTION
size_t __wrap_strlen(const char *s)
{
	return _strlen(s);
}

LIBC_ROM_TEXT_SECTION
int __wrap_strcmp(const char *cs, const char *ct)
{
	return _strcmp(cs, ct);
}

LIBC_ROM_TEXT_SECTION
int __wrap_stricmp(const char *cs, const char *ct)
{
	return _stricmp(cs, ct);
}

LIBC_ROM_TEXT_SECTION
u8 *__wrap_strupr(IN  u8  *string)
{
	return _strupr(string);
}

LIBC_ROM_TEXT_SECTION
unsigned long __wrap_strtoul(const char *s, char **ptr, int base)
{
	return _strtoul(s, ptr, base);
}

