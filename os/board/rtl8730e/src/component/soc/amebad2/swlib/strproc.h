/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifndef _STRPROC_H_
#define _STRPROC_H_

#include <stddef.h> /* for size_t */
#include <stdarg.h>
#include "platform_autoconf.h"
#include "basic_types.h"

extern _LONG_CALL_ int _strcmp(const char *cs, const char *ct);
extern _LONG_CALL_ size_t _strlen(const char *s);
extern _LONG_CALL_ int _stricmp(const char *str1, const char *str2);
extern _LONG_CALL_ u8 *_strupr(IN  u8  *string);
extern _LONG_CALL_ unsigned long _strtoul(const char *cp, char **endp, int base);
extern _LONG_CALL_ char *strsep(char **stringP, const char *delim);
extern _LONG_CALL_ char *strncpy(char *dest, const char *src, size_t n);

extern  int __wrap_strcmp(const char *cs, const char *ct);
extern  size_t __wrap_strlen(const char *s);
extern  int __wrap_stricmp(const char *str1, const char *str2);
extern  u8 *__wrap_strupr(IN  u8  *string);
extern  unsigned long __wrap_strtoul(const char *cp, char **endp, int base);
extern _LONG_CALL_ char *_strsep(char **stringP, const char *delim);
extern _LONG_CALL_ char *_strncpy(char *dest, const char *src, size_t n);

extern _LONG_CALL_ u8 _char2num(u8 ch);
extern _LONG_CALL_ u8 _2char2dec(u8 hch, u8 lch);
extern _LONG_CALL_ u8 _2char2hex(u8 hch, u8 lch);

/*
 * Fast implementation of tolower() for internal usage. Do not use in your
 * code.
 */
#ifndef _tolower
static inline char _tolower(const char c)
{
	return c | 0x20;
}
#endif

/* Fast check for octal digit */
static inline int isodigit(const char c)
{
	return c >= '0' && c <= '7';
}

#endif
