/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * This file is based on Linux kernel KASan implementation
 *
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
 * Author: Andrey Ryabinin <ryabinin.a.a@gmail.com>
 *
 * Some code borrowed from https://github.com/xairy/kasan-prototype by
 *        Andrey Konovalov <adech.fo@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 ****************************************************************************/

#include <tinyara/config.h>
#include <unistd.h>
#include <assert.h>
#include <debug.h>
#include "asan.h"

__attribute__((no_sanitize_address))
static void *_memset(void *s, int c, size_t n)
{
	unsigned char *p = (unsigned char *)s;
	while (n-- > 0) {
		*p++ = c;
	}
	return s;
}

__attribute__((no_sanitize_address))
void asan_clean_shadow(void)
{
	unsigned char *addr = (unsigned char *)asan_mem_to_shadow((void *)CONFIG_RAM_START);
	int n = CONFIG_RAM_SIZE >> ASAN_SHADOW_SCALE_SHIFT;
	while (n > 0) {
		*(uint32_t *)addr = 0;
		addr += 4;
		n -= 4;
	}
}

/*
 * Poisons the shadow memory for 'size' bytes starting from 'addr'.
 * Memory addresses should be aligned to ASAN_SHADOW_SCALE_SIZE.
 */
__attribute__((no_sanitize_address))
static void asan_poison_shadow(const void *address, size_t size, u8 value)
{
	void *shadow_start, *shadow_end;

	shadow_start = asan_mem_to_shadow(address);
	shadow_end = asan_mem_to_shadow(address + size);

	_memset(shadow_start, value, shadow_end - shadow_start);
}

__attribute__((no_sanitize_address))
void asan_poison_heap(const void *address, size_t size)
{
	asan_poison_shadow(address, size, ASAN_MALLOC_REDZONE);
}

__attribute__((no_sanitize_address))
void asan_poison_free(const void *address, size_t size)
{
	asan_poison_shadow(address, size, ASAN_MALLOC_FREE);
}

__attribute__((no_sanitize_address))
void asan_unpoison_shadow(const void *address, size_t size)
{
	asan_poison_shadow(address, size, 0);

	if (size & ASAN_SHADOW_MASK) {
		u8 *shadow = (u8 *)asan_mem_to_shadow(address + size);
		*shadow = size & ASAN_SHADOW_MASK;
	}
}

__attribute__((no_sanitize_address))
void asan_unpoison_heap(const void *address, size_t size)
{
	asan_unpoison_shadow(address, size);
}

/*
 * All functions below always inlined so compiler could
 * perform better optimizations in each of __asan_loadX/__assn_storeX
 * depending on memory access size X.
 */
__attribute__((no_sanitize_address))
static inline bool memory_is_poisoned_1(unsigned long addr)
{
	s8 shadow_value = *(s8 *)asan_mem_to_shadow((void *)addr);

	if (shadow_value) {
		s8 last_accessible_byte = addr & ASAN_SHADOW_MASK;
		return last_accessible_byte >= shadow_value;
	}

	return false;
}

__attribute__((no_sanitize_address))
static inline bool memory_is_poisoned_2(unsigned long addr)
{
	u16 *shadow_addr = (u16 *)asan_mem_to_shadow((void *)addr);

	if (*shadow_addr) {
		if (memory_is_poisoned_1(addr + 1)) {
			return true;
		}

		if (((addr + 1) & ASAN_SHADOW_MASK) != 0) {
			return false;
		}

		return (*(u8 *)shadow_addr);
	}

	return false;
}

__attribute__((no_sanitize_address))
static inline bool memory_is_poisoned_4(unsigned long addr)
{
	u16 *shadow_addr = (u16 *)asan_mem_to_shadow((void *)addr);

	if (*shadow_addr) {
		if (memory_is_poisoned_1(addr + 3)) {
			return true;
		}

		if (((addr + 3) & ASAN_SHADOW_MASK) >= 3) {
			return false;
		}

		return (*(u8 *)shadow_addr);
	}

	return false;
}

__attribute__((no_sanitize_address))
static inline bool memory_is_poisoned_8(unsigned long addr)
{
	u16 *shadow_addr = (u16 *)asan_mem_to_shadow((void *)addr);

	if (*shadow_addr) {
		if (memory_is_poisoned_1(addr + 7)) {
			return true;
		}

		if (((addr + 7) & ASAN_SHADOW_MASK) >= 7) {
			return false;
		}

		return (*(u8 *)shadow_addr);
	}

	return false;
}

__attribute__((no_sanitize_address))
static inline bool memory_is_poisoned_16(unsigned long addr)
{
	u32 *shadow_addr = (u32 *)asan_mem_to_shadow((void *)addr);

	if (*shadow_addr) {
		u16 shadow_first_bytes = *(u16 *)shadow_addr;
		s8 last_byte = (addr + 15) & ASAN_SHADOW_MASK;

		if (shadow_first_bytes) {
			return true;
		}

		if (!last_byte) {
			return false;
		}

		return memory_is_poisoned_1(addr + 15);
	}

	return false;
}

__attribute__((no_sanitize_address))
static inline unsigned long bytes_is_zero(const u8 *start, size_t size)
{
	while (size) {
		if (*start) {
			return (unsigned long)start;
		}
		start++;
		size--;
	}

	return 0;
}

__attribute__((no_sanitize_address))
static inline unsigned long memory_is_zero(const void *start, const void *end)
{
	unsigned int words;
	unsigned long ret;
	unsigned int prefix = (unsigned long)start % 8;

	if (end - start <= 16) {
		return bytes_is_zero(start, end - start);
	}

	if (prefix) {
		prefix = 8 - prefix;
		ret = bytes_is_zero(start, prefix);
		if (ret) {
			return ret;
		}
		start += prefix;
	}

	words = (end - start) / 8;
	while (words) {
		if (*(u64 *)start) {
			return bytes_is_zero(start, 8);
		}
		start += 8;
		words--;
	}

	return bytes_is_zero(start, (end - start) % 8);
}

__attribute__((no_sanitize_address))
static inline bool memory_is_poisoned_n(unsigned long addr, size_t size)
{
	unsigned long ret;

	ret = memory_is_zero(asan_mem_to_shadow((void *)addr), asan_mem_to_shadow((void *)addr + size - 1) + 1);

	if (ret) {
		unsigned long last_byte = addr + size - 1;
		s8 *last_shadow = (s8 *)asan_mem_to_shadow((void *)last_byte);
		if (ret != (unsigned long)last_shadow || ((last_byte & ASAN_SHADOW_MASK) >= *last_shadow)) {
			return true;
		}
	}
	return false;
}

__attribute__((no_sanitize_address))
static inline bool memory_is_poisoned(unsigned long addr, size_t size)
{
	if (size <= 16 && (!(size % 2) || size == 1)) {
		switch (size) {
		case 1:
			return memory_is_poisoned_1(addr);
		case 2:
			return memory_is_poisoned_2(addr);
		case 4:
			return memory_is_poisoned_4(addr);
		case 8:
			return memory_is_poisoned_8(addr);
		case 16:
			return memory_is_poisoned_16(addr);
		default:
			PANIC();
		}
	}

	return memory_is_poisoned_n(addr, size);
}

__attribute__((always_inline, no_sanitize_address))
static inline void check_memory_region(unsigned long addr, size_t size, bool write)
{
	/* Handle MMIO */
	if (addr < CONFIG_RAM_START || addr > CONFIG_RAM_END || up_interrupt_context()) {
		return;
	}

	struct asan_access_info info;

	if (size == 0) {
		return;
	}
	if ((void *)addr < asan_shadow_to_mem((void *)ASAN_SHADOW_OFFSET)) {
		info.access_addr = (void *)addr;
		info.access_size = size;
		info.is_write = write;
		info.ip = _RET_IP_;
		asan_report_user_access(&info);
		return;
	}
	if (!memory_is_poisoned(addr, size)) {
		return;
	}
	asan_report(addr, size, write, _RET_IP_);
}

#define DEFINE_ASAN_LOAD_STORE(size)			\
__attribute__((no_sanitize_address))			\
void __asan_load##size(unsigned long addr)		\
{							\
	check_memory_region(addr, size, false);		\
}							\
__alias(__asan_load##size)				\
void __asan_load##size##_noabort(unsigned long);	\
__attribute__((no_sanitize_address))			\
void __asan_store##size(unsigned long addr)		\
{							\
	check_memory_region(addr, size, true);		\
}							\
__alias(__asan_store##size)				\
void __asan_store##size##_noabort(unsigned long);

DEFINE_ASAN_LOAD_STORE(1);
DEFINE_ASAN_LOAD_STORE(2);
DEFINE_ASAN_LOAD_STORE(4);
DEFINE_ASAN_LOAD_STORE(8);
DEFINE_ASAN_LOAD_STORE(16);
__attribute__((no_sanitize_address))
void __asan_loadN(unsigned long addr, size_t size)
{
	check_memory_region(addr, size, false);
}

__alias(__asan_loadN)
void __asan_loadN_noabort(unsigned long, size_t);
__attribute__((no_sanitize_address))
void __asan_storeN(unsigned long addr, size_t size)
{
	check_memory_region(addr, size, true);
}

__alias(__asan_storeN)
void __asan_storeN_noabort(unsigned long, size_t);

/* to shut up compiler complaints */
void __asan_handle_no_return(void)
{
}
