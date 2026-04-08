/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/

/*
 * s1_utils.c
 *
 * Optimized memcpy/memset implementations for the bootloader.
 * These versions use word-sized accesses for aligned memory blocks
 * to improve performance on 32-bit ARM processors.
 */
#include "s1_utils.h"

void *s1_memcpy(void *dest, const void *src, size_t n)
{
	uint8_t *d = (uint8_t *)dest;
	const uint8_t *s = (const uint8_t *)src;

	/* Align destination pointer */
	while (n > 0 && ((uintptr_t)d & (sizeof(uint32_t) - 1)) != 0)
	{
		*d++ = *s++;
		n--;
	}

	/* Copy word by word */
	uint32_t *wd = (uint32_t *)d;
	const uint32_t *ws = (const uint32_t *)s;
	while (n >= sizeof(uint32_t))
	{
		*wd++ = *ws++;
		n -= sizeof(uint32_t);
	}

	/* Copy remaining bytes */
	d = (uint8_t *)wd;
	s = (const uint8_t *)ws;
	while (n > 0)
	{
		*d++ = *s++;
		n--;
	}

	return dest;
}

void *s1_memset(void *s, int c, size_t n)
{
	uint8_t *p = (uint8_t *)s;
	uint8_t val = (uint8_t)c;
	uint32_t word_val = (uint32_t)c << 24 | (uint32_t)c << 16 | (uint32_t)c << 8 | c;

	/* Align pointer */
	while (n > 0 && ((uintptr_t)p & (sizeof(uint32_t) - 1)) != 0)
	{
		*p++ = val;
		n--;
	}

	/* Write word by word */
	uint32_t *wp = (uint32_t *)p;
	while (n >= sizeof(uint32_t))
	{
		*wp++ = word_val;
		n -= sizeof(uint32_t);
	}

	/* Write remaining bytes */
	p = (uint8_t *)wp;
	while (n > 0)
	{
		*p++ = val;
		n--;
	}

	return s;
}