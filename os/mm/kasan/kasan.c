/****************************************************************************
 *
 * Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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
 * Included Files
 ****************************************************************************/

#include <tinyara/mm/kasan.h>
#include <tinyara/mm/mm.h>
#include <string.h>		/* for memset func */

#include "kasan.h"

#ifdef CONFIG_KASAN

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Type Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/**
 * @brief
 *  start address of .data section
 *
 */
unsigned long datastart;

/**
 * @brief
 *  end address of .data section
 *
 */
unsigned long dataend;

/**
 * @brief
 *  start address of .bss section
 *
 */
unsigned long bssstart;

/**
 * @brief
 *  end address of .bss section
 *
 */
unsigned long bssend;

/**
 * @brief
 *  start address of idle stack region
 *
 */
unsigned long idlestackstart;

/**
 * @brief
 *  end address of idle stack region
 *
 */
unsigned long idlestackend;

/**
 * @brief
 *  start address of heap region
 *
 */
unsigned long heapstart;

/**
 * @brief
 *  end address of heap region
 *
 */
unsigned long heapend;

/**
 * @brief
 *  start address of .data section shadow region
 *
 */
unsigned long datastart_shadow;

/**
 * @brief
 *  end address of .data section shadow region
 *
 */
unsigned long dataend_shadow;

/**
 * @brief
 *  start address of .bss section shadow region
 *
 */
unsigned long bssstart_shadow;

/**
 * @brief
 *  end address of .bss section shadow region
 *
 */
unsigned long bssend_shadow;

/**
 * @brief
 *  start address of idle stack shadow region
 *
 */
unsigned long idlestackstart_shadow;

/**
 * @brief
 *  end address of idle stack shadow region
 *
 */
unsigned long idlestackend_shadow;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/**
 * @brief
 *  Check shadow memory related to memory region started at 'addr' with size 1.
 *
 * @param addr address of memory region
 *
 * @return
 *  true if memory is poisoned and false otherwise.
 *
 */
static KASAN_INLINE bool memory_is_poisoned_1(unsigned long addr)
{
	signed char last_byte;
	signed char shadow_value =
		*(signed char *)mem_to_shadow(addr);

	if (shadow_value) {
		last_byte = addr & KASAN_SHADOW_MASK;
		return (last_byte >= shadow_value);
	}

	return false;
}

/**
 * @brief
 *  Check shadow memory related to memory region started at 'addr' with size 2.
 *
 * @param addr address of memory region
 *
 * @return
 *  true if memory is poisoned and false otherwise.
 *
 */
static KASAN_INLINE bool memory_is_poisoned_2(unsigned long addr)
{
	signed short *shadow_addr =
		(signed short *)mem_to_shadow(addr);

	if (*shadow_addr) {
		if (memory_is_poisoned_1(addr + 1))
			return true;

		if (((addr + 1) & KASAN_SHADOW_MASK) >= 1)
			return false;

		return *(signed char *)shadow_addr;
	}

	return false;
}

/**
 * @brief
 *  Check shadow memory related to memory region started at 'addr' with size 4.
 *
 * @param addr address of memory region
 *
 * @return
 *  true if memory is poisoned and false otherwise.
 *
 */
static KASAN_INLINE bool memory_is_poisoned_4(unsigned long addr)
{
	signed short *shadow_addr =
		(signed short *)mem_to_shadow(addr);

	if (*shadow_addr) {
		if (memory_is_poisoned_1(addr + 3))
			return true;

		if (((addr + 3) & KASAN_SHADOW_MASK) >= 3)
			return false;

		return *(signed char *)shadow_addr;
	}

	return false;
}

/**
 * @brief
 *  Check shadow memory related to memory region started at 'addr' with size 8.
 *
 * @param addr address of memory region
 *
 * @return
 *  true if memory is poisoned and false otherwise.
 *
 */
static KASAN_INLINE bool memory_is_poisoned_8(unsigned long addr)
{
	signed short *shadow_addr =
		(signed short *)mem_to_shadow(addr);

	if (*shadow_addr) {
		if (memory_is_poisoned_1(addr + 7))
			return true;

		if (MM_IS_ALIGNED(addr, KASAN_SHADOW_SCALE_SIZE))
			return false;

		return *(signed char *)shadow_addr;
	}

	return false;
}

/**
 * @brief
 *  Check shadow memory related to memory region started at 'addr' with size 16.
 *
 * @param addr address of memory region
 *
 * @return
 *  true if memory is poisoned and false otherwise.
 *
 */
static KASAN_INLINE bool memory_is_poisoned_16(unsigned long addr)
{
	signed int *shadow_addr = (signed int *)mem_to_shadow(addr);
	signed short shadow_first_bytes;

	if (*shadow_addr) {
		shadow_first_bytes = *(signed short *)shadow_addr;

		if (shadow_first_bytes)
			return true;

		if (MM_IS_ALIGNED(addr, KASAN_SHADOW_SCALE_SIZE))
			return false;

		return memory_is_poisoned_1(addr + 15);
	}

	return false;
}

/**
 * @brief
 *  Find first poisoned byte in memory region started at 'start' with size 'size'.
 *
 * @param start address of memory region
 * @param size size of memory region
 *
 * @return
 *  Address of first poisoned byte or 0 otherwise.
 *
 */
static KASAN_INLINE
unsigned long first_poisoned_byte(unsigned long start,
				  size_t size)
{
	while (size) {
		if (*(signed char *)start)
			return start;

		++start;
		--size;
	}

	return 0;
}

/**
 * @brief
 *  Find address of first poisoned byte in memory region started at 'start' and ended at 'end'.
 *
 * @param start first address of memory region
 * @param end last address of memory region
 *
 * @return
 *  Address of first poisoned byte or 0 otherwise.
 *
 */
static KASAN_INLINE unsigned long first_poisoned_addr(unsigned long start,
							unsigned long end)
{
	unsigned int words, prefix;
	unsigned long poisoned_addr;
	size_t size;

	size = end - start;
	if (size <= 16)
		return first_poisoned_byte(start, size);

	prefix = start % 8;
	if (prefix) {
		prefix = 8 - prefix;

		poisoned_addr = first_poisoned_byte(start, prefix);
		if (poisoned_addr) {
			return poisoned_addr;
		}

		start += prefix;
	}

	words = size / 8;
	while (words) {
		if (*(long long *)start)
			return first_poisoned_byte(start, 8);

		start += 8;
		--words;
	}

	return first_poisoned_byte(start, size % 8);
}

/**
 * @brief
 *  Check shadow memory related to memory region started at 'addr' with size 'size'.
 *
 * @param addr address of memory region
 * @param size size of memory region
 *
 * @return
 *  true if memory is poisoned and false otherwise.
 *
 */
static KASAN_INLINE bool memory_is_poisoned_n(unsigned long addr, size_t size)
{
	unsigned long poisoned_addr_shadow, last_byte, last_shadow;

	poisoned_addr_shadow = first_poisoned_addr(mem_to_shadow(addr),
						mem_to_shadow(addr + size - 1)
						+ 1);

	if (poisoned_addr_shadow) {
		last_byte = addr + size - 1;
		last_shadow = mem_to_shadow(last_byte);

		if ((poisoned_addr_shadow != last_shadow) ||
			((last_byte & KASAN_SHADOW_MASK) >=
				*(signed char *)last_shadow))
			return true;
	}

	return false;
}

/**
 * @brief
 *  Check shadow memory related to memory region started at 'addr' with size 'size'.
 *
 * @param addr address of memory region
 * @param size size of memory region
 *
 * @return
 *  true if memory is poisoned and false otherwise.
 *
 */
static KASAN_INLINE bool memory_is_poisoned(unsigned long addr, size_t size)
{
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
		return memory_is_poisoned_n(addr, size);
	}
}

/**
 * @brief
 *  Check shadow memory related to memory region started at 'addr' with size 'size'
 *  and if memory region is poisoned, then print report.
 *
 * @param addr address of memory region
 * @param size size of memory region
 * @param write type of access (true for write and false for read)
 * @param ret_ip address of the instruction which caused the error
 *
 * @return
 *  None
 *
 */
static KASAN_INLINE void check_mem_reg(unsigned long addr,
					 size_t size, bool write,
					 unsigned long ret_ip)
{
	if (size == 0 || !UNDER_SHADOW(addr))
		return;

	if (memory_is_poisoned(addr, size))
		kasan_report(addr, size, write, ret_ip);
}

/**
 * @brief
 *  Register global varialbe by unpoisoning memory related to variable
 *  and poisoning redzone after variable.
 *
 * @param global pointer to struct which describes global variable
 *
 * @return
 *  None
 *
 */
static KASAN_INLINE void register_global(const struct kasan_global *global)
{
	if (!global || !IS_GLOB_ADDR(global->beg))
		return;

	kasan_unpoison((unsigned long)global->beg, global->size);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
 * @brief
 *  Return shadow value associated with address
 *
 * @param addr memory address
 *
 * @return
 *  Shadow value
 *
 */
signed char kasan_shadow_val(unsigned long addr)
{
	return *(signed char *)mem_to_shadow(addr);
}

/**
 * @brief
 *  Poison shadow memory related to memory region.
 *
 * @param addr address of memory region
 * @param size size of memory region
 * @param value byte value for related shadow memory region
 *
 * @return
 *   None
 *
 * @attention
 *  Memory address should be aligned to KASAN_SHADOW_SCALE_SIZE.
 *
 */
void kasan_poison(unsigned long addr, size_t size, signed char value)
{
	unsigned long shadow_start, shadow_end;

	shadow_start = mem_to_shadow(addr);
	shadow_end = mem_to_shadow(addr + size);

	memset((void *)shadow_start, value, shadow_end - shadow_start);
}

/**
 * @brief
 *  Unpoison shadow memory related to memory region.
 *
 * @param addr address of memory region
 * @param size size of memory region
 *
 * @return
 *   None
 *
 * @attention
 *  Memory address should be aligned to KASAN_SHADOW_SCALE_SIZE.
 *
 */
void kasan_unpoison(unsigned long addr, size_t size)
{
	kasan_poison(addr, size, KASAN_ANTIDOTE);

	if (!MM_IS_ALIGNED(size, KASAN_SHADOW_SCALE_SIZE)) {
		signed char *shadow =
			(signed char *)mem_to_shadow(addr + size);
		*shadow = (signed char)(size & KASAN_SHADOW_MASK);
	}
}

/**
 * @brief
 *  Poison shadow memory related to heap memory region.
 *
 * @param addr address of memory region
 * @param size size of memory region
 * @param value value for bytes in shadow memory region
 *
 * @return
 *   None
 *
 * @attention
 *  Memory address should be aligned to KASAN_SHADOW_SCALE_SIZE.
 *
 */
void kasan_poison_heap(unsigned long addr, size_t size, signed char value)
{
	if (!addr || !IS_HEAP_ADDR((unsigned long)addr))
		return;

	kasan_poison(addr, size, value);
}

/**
 * @brief
 *  Unpoison shadow memory related to heap memory region.
 *
 * @param addr address of memory region
 * @param size size of memory region
 *
 * @return
 *   None
 *
 * @attention
 *  - address should be aligned to KASAN_SHADOW_SCALE_SIZE
 *  - address should belongs to heap memory
 *
 */
void kasan_unpoison_heap(unsigned long addr, size_t size)
{
	if (!addr || !IS_HEAP_ADDR(addr))
		return;

	kasan_unpoison(addr, size);
}

/**
 * @brief
 *  Initialize KASan shadow memory by filling it with KASAN_REDZONE byte.
 *
 * @return
 *   None
 *
 */
void kasan_init(void)
{
	unsigned long p = KASAN_SHADOW_START;
	signed char val;
	int n = KASAN_SHADOW_SIZE;

	kllvdbg(KASAN_PREFIX "initialization\n");

	/* fill shadow memory with redzone */
	while (n--) {
		/*
		 * skip idle thread stack shadow region,
		 * because this stack already may be used.
		 */
		if (idlestackstart_shadow <= p && p < idlestackend_shadow) {
			p++;
			continue;
		}

		val = KASAN_REDZONE;

		if ((datastart_shadow <= p && p < dataend_shadow) ||
			(bssstart_shadow <= p && p < bssend_shadow)) {
			val = KASAN_GLOBAL_REDZONE;
		}

		*(signed char *)p++ = val;
	}
}

#ifdef CONFIG_KASAN_GLOBALS
/**
 * @brief
 *  Get first and last addresses of .data section. This needed for correct work of
 *  UNDER_SHADOW macro.
 *
 * @param start first address of .data section
 * @param end last address of .data section
 *
 * @return
 *   None
 *
 * @note
 *  Useful only with CONFIG_KASAN_GLOBALS option.
 *
 */
void kasan_set_databounds(unsigned long start, unsigned long end)
{
	datastart = start;
	dataend = end;

	datastart_shadow = mem_to_shadow(start);
	dataend_shadow = mem_to_shadow(end);
}

/**
 * @brief
 *  Get first and last addresses of .bss section. This needed for correct work of
 *  UNDER_SHADOW macro.
 *
 * @param start first address of .bss section
 * @param end last address of .bss section
 *
 * @return
 *   None
 *
 * @note
 *  Useful only with CONFIG_KASAN_GLOBALS option.
 *
 */
void kasan_set_bssbounds(unsigned long start, unsigned long end)
{
	bssstart = start;
	bssend = end;

	bssstart_shadow = mem_to_shadow(start);
	bssend_shadow = mem_to_shadow(end);
}
#endif	/* CONFIG_KASAN_GLOBALS */

#ifdef CONFIG_KASAN_IDLESTACK
/**
 * @brief
 *  Get first and last stack addresses. This needed for correct work of
 *  UNDER_SHADOW macro.
 *
 * @param start first stack address
 * @param end last stack address
 *
 * @return
 *   None
 *
 * @note
 *  Useful only with CONFIG_KASAN_IDLESTACK option.
 *
 */
void kasan_set_idlestackbounds(unsigned long start, unsigned long end)
{
	idlestackstart = start;
	idlestackend = end;

	idlestackstart_shadow = mem_to_shadow(start);
	idlestackend_shadow = mem_to_shadow(end);
}
#endif	/* CONFIG_KASAN_IDLESTACK */

#ifdef CONFIG_KASAN_HEAP
/**
 * @brief
 *  Get first and last heap addresses. This needed for correct work of
 *  UNDER_SHADOW macro.
 *
 * @param start first heap address
 * @param end last heap address
 *
 * @return
 *   None
 *
 * @note
 *  Useful only with CONFIG_KASAN_HEAP option.
 *
 */
void kasan_set_heapbounds(unsigned long start, unsigned long end)
{
	heapstart = start;
	heapend   = end;
}
#endif	/* CONFIG_KASAN_HEAP */

/****************************************************************************
 * Following doxygen comments relates to funcitons defined through ASAN_LOAD macro.
 * Generate comments inside macro isn't possible because C preprocessor removes comments during macro expansion.
 ****************************************************************************/

/**
 * @fn void __asan_load1(unsigned long addr)
 *
 * @brief
 *  Check permissibility of load instructions
 *  to memory region started from 'addr'.
 *
 * @param addr address of memory region with size 1
 *
 * @return
 *  None
 *
 */

/**
 * @fn void __asan_load2(unsigned long addr)
 *
 * @brief
 *  Check permissibility of load instructions
 *  to memory region started from 'addr'.
 *
 * @param addr address of memory region with size 2
 *
 * @return
 *  None
 *
 */

/**
 * @fn void __asan_load4(unsigned long addr)
 *
 * @brief
 *  Check permissibility of load instructions
 *  to memory region started from 'addr'.
 *
 * @param addr address of memory region with size 4
 *
 * @return
 *  None
 *
 */

/**
 * @fn void __asan_load8(unsigned long addr)
 *
 * @brief
 *  Check permissibility of load instructions
 *  to memory region started from 'addr'.
 *
 * @param addr address of memory region with size 8
 *
 * @return
 *  None
 *
 */

/**
 * @fn void __asan_load16(unsigned long addr)
 *
 * @brief
 *  Check permissibility of load instructions
 *  to memory region started from 'addr'.
 *
 * @param addr address of memory region with size 16
 *
 * @return
 *  None
 *
 */

/**
 * @brief
 *  Defined __asan_load* and __asan_load*_noabort functions
 *  for compiler instrumentation.
 */
#define ASAN_LOAD(size)					\
	void __asan_load##size(unsigned long addr)		\
	{							\
		check_mem_reg(addr, size, false,		\
				(unsigned long)RET_IP);		\
	}							\
											\
	alias(__asan_load##size)				\
	void __asan_load##size##_noabort(unsigned long addr)

ASAN_LOAD(1);
ASAN_LOAD(2);
ASAN_LOAD(4);
ASAN_LOAD(8);
ASAN_LOAD(16);

/**
 * @brief
 *  Check permissibility of load instructions
 *  to memory region started from 'addr' wit size 'size'.
 *
 * @param addr address of memory region
 * @param size size of memory reigon
 *
 * @return
 *  None
 *
 */
void __asan_loadN(unsigned long addr, size_t size)
{
	check_mem_reg(addr, size, false,
			(unsigned long)RET_IP);
}

alias(__asan_loadN)
void __asan_loadN_noabort(unsigned long addr, size_t size);

/****************************************************************************
 * Following doxygen comments relates to funcitons defined through ASAN_STORE macro.
 * Generate comments inside macro isn't possible because C preprocessor removes comments during macro expansion.
 ****************************************************************************/

/**
 * @fn void __asan_store1(unsigned long addr)
 *
 * @brief
 *  Check permissibility of store instructions
 *  to memory region started from 'addr'.
 *
 * @param addr address of memory region with size 1
 *
 * @return
 *  None
 *
 */

/**
 * @fn void __asan_store2(unsigned long addr)
 *
 * @brief
 *  Check permissibility of store instructions
 *  to memory region started from 'addr'.
 *
 * @param addr address of memory region with size 2
 *
 * @return
 *  None
 *
 */

/**
 * @fn void __asan_store4(unsigned long addr)
 *
 * @brief
 *  Check permissibility of store instructions
 *  to memory region started from 'addr'.
 *
 * @param addr address of memory region with size 4
 *
 * @return
 *  None
 *
 */

/**
 * @fn void __asan_store8(unsigned long addr)
 *
 * @brief
 *  Check permissibility of store instructions
 *  to memory region started from 'addr'.
 *
 * @param addr address of memory region with size 8
 *
 * @return
 *  None
 *
 */

/**
 * @fn void __asan_store16(unsigned long addr)
 *
 * @brief
 *  Check permissibility of store instructions
 *  to memory region started from 'addr'.
 *
 * @param addr address of memory region with size 16
 *
 * @return
 *  None
 *
 */

/**
 * @brief
 *  Defined __asan_store* and __asan_store*_noabort functions
 *  for compiler instrumentation.
 */
#define ASAN_STORE(size)						\
	void __asan_store##size(unsigned long addr)			\
	{								\
		check_mem_reg(addr, size, true,				\
				(unsigned long)RET_IP);			\
	}								\
									\
	alias(__asan_store##size)					\
	void __asan_store##size##_noabort(unsigned long addr)

ASAN_STORE(1);
ASAN_STORE(2);
ASAN_STORE(4);
ASAN_STORE(8);
ASAN_STORE(16);

/**
 * @brief
 *  Check permissibility of store instructions
 *  to memory region started from 'addr'.
 *
 * @param addr address of memory region
 * @param size size of memory region
 *
 * @return
 *  None
 *
 */
void __asan_storeN(unsigned long addr, size_t size)
{
	check_mem_reg(addr, size, true,
			(unsigned long)RET_IP);
}

alias(__asan_storeN)
void __asan_storeN_noabort(unsigned long addr, size_t size);

/**
 * @brief
 *  Register global variables in shadow memory by unpoisoning variable and
 *  poisoning redzone right after it.
 *
 * @param globals array of global variables
 * @param size size of array
 *
 * @return
 *  None
 *
 */
void __asan_register_globals(const struct kasan_global *globals, size_t size)
{
	int i;

	for (i = 0; i < size; ++i)
		register_global(&globals[i]);
}

/**
 * @brief
 *  Poison shadow memory related to stack memory region.
 *
 * @param addr address of memory region
 * @param size size of memory region
 *
 * @return
 *  None
 *
 */
void __asan_poison_stack_memory(unsigned long addr, size_t size)
{
	if (!(IS_IDLESTACK_ADDR(addr) || IS_HEAP_ADDR(addr)))
		return;

	kasan_poison(addr, MM_ALIGN_UP_COMMON(size, KASAN_SHADOW_SCALE_SIZE),
			KASAN_USE_AFTER_SCOPE);
}

alias(__asan_poison_stack_memory)
void kasan_poison_stack(unsigned long addr, size_t size);

/**
 * @brief
 *  Unpoison shadow memory related to stack memory region.
 *
 * @param addr address of memory region
 * @param size size of memory region
 *
 * @return
 *  None
 *
 */
void __asan_unpoison_stack_memory(unsigned long addr, size_t size)
{
	if (!(IS_IDLESTACK_ADDR(addr) || IS_HEAP_ADDR(addr)))
		return;

	kasan_unpoison(addr, size);
}

alias(__asan_unpoison_stack_memory)
void kasan_unpoison_stack(unsigned long addr, size_t size);

void __asan_handle_no_return(void) {}

void __asan_unregister_globals(void) {}

#endif	/* CONFIG_KASAN */
