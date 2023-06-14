/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifndef _MEM_PROC_H_
#define _MEM_PROC_H_

#include <basic_types.h>

/* change name from hal_misc.h */
extern _LONG_CALL_ void *_memset(void *s, int c, size_t n);
extern _LONG_CALL_ void *_memcpy(void *s1, const void *s2, size_t n);
extern _LONG_CALL_ int _memcmp(const void *av, const void *bv, size_t len);
extern _LONG_CALL_ void *_memchr(const void *src_void, int c, size_t length);
extern _LONG_CALL_ void *_memmove(void *dst_void, const void *src_void, size_t length);

extern  void *__wrap_memset(void *s, int c, size_t n);
extern  void *__wrap_memcpy(void *s1, const void *s2, size_t n);
extern  int __wrap_memcmp(const void *av, const void *bv, size_t len);
extern  void *__wrap_memchr(const void *src_void, int c, size_t length);
extern  void *__wrap_memmove(void *dst_void, const void *src_void, size_t length);

void memcpy_gdma_init(void);
int memcpy_gdma(void *dest, void *src, u32 size);

#endif  //_MEM_PROC_H_
