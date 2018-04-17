/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

#ifndef __MM_KASAN_KASAN_H
#define __MM_KASAN_KASAN_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <sys/types.h>
#include <stdbool.h>

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

#ifdef CONFIG_KASAN

#ifdef CONFIG_KASAN_STACK
#if CONFIG_IDLETHREAD_STACKSIZE < CONFIG_KASAN_MIN_STACK
#error "CONFIG_IDLETHREAD_STACKSIZE < CONFIG_KASAN_MIN_STACK"
#endif

#if CONFIG_USERMAIN_STACKSIZE < CONFIG_KASAN_MIN_STACK
#error "CONFIG_USERMAIN_STACKSIZE < CONFIG_KASAN_MIN_STACK"
#endif

#if CONFIG_PTHREAD_STACK_DEFAULT < CONFIG_KASAN_MIN_STACK
#error "CONFIG_PTHREAD_STACK_DEFAULT < CONFIG_KASAN_MIN_STACK"
#endif
#endif	/* CONFIG_KASAN_STACK */

/**
 * @brief
 *  Always inline KASan functions
 *
 */

#ifdef CONFIG_KASAN_FUNCS_INLINE
#define KASAN_INLINE inline inline_function
#else
#define KASAN_INLINE
#endif	/* CONFIG_KASAN_FUNCS_INLINE */

extern unsigned long datastart;
extern unsigned long dataend;
extern unsigned long bssstart;
extern unsigned long bssend;
extern unsigned long idlestackstart;
extern unsigned long idlestackend;
extern unsigned long heapstart;
extern unsigned long heapend;

extern unsigned long idlestackstart_shadow;
extern unsigned long idlestackend_shadow;

/**
 * @brief
 *  Check that address belongs to .data or .bss section
 *
 * @param addr address
 *
 */
#define IS_GLOB_ADDR(addr)					\
	(((datastart  <= (addr)) && ((addr) < dataend))  ||	\
	 ((bssstart   <= (addr)) && ((addr) < bssend)))

/**
 * @brief
 *  Check that address belongs to idle stack region
 *
 * @param addr address
 *
 */
#define IS_IDLESTACK_ADDR(addr)				\
	((idlestackstart <= (addr)) && ((addr) < idlestackend))

/**
 * @brief
 *  Check that address belongs to heap region
 *
 * @param addr address
 *
 */
#define IS_HEAP_ADDR(addr)				\
	((heapstart  <= (addr)) && ((addr) < heapend))

#define IS_SHAD_ADDR(addr)						\
	((KASAN_SHADOW_START <= (addr)) && ((addr) <= KASAN_SHADOW_END))

/**
 * @brief
 *  Check that address has related shadow region
 *
 * @param addr address
 *
 */
#define UNDER_SHADOW(addr)						\
	(IS_GLOB_ADDR(addr) || IS_IDLESTACK_ADDR(addr) || IS_HEAP_ADDR(addr))

/**
 * @brief
 *  Absence of redzone
 *
 */
#define KASAN_ANTIDOTE		   0x00

/**
 * @brief
 *  Redzone for heap
 *
 */
#define KASAN_REDZONE		   0xFE

/**
 * @brief
 *  Redzone for global variables
 *
 */
#define KASAN_GLOBAL_REDZONE	   0xFA

/**
 * @brief
 *  Redzone for memory in quarantine
 *
 */
#define KASAN_QUARANTINE           0xFB

/**
 * @brief
 *  Redzone for stack variables
 *
 */
#define KASAN_STACK_LEFT           0xF1

/**
 * @brief
 *  Redzone for stack variables
 *
 */
#define KASAN_STACK_MID            0xF2

/**
 * @brief
 *  Redzone for stack variables
 *
 */
#define KASAN_STACK_RIGHT          0xF3

/**
 * @brief
 *  Redzone for stack variables
 *
 */
#define KASAN_STACK_PARTIAL        0xF4

/**
 * @brief
 *  Redzone for stack variable out of it scope
 *
 */
#define KASAN_USE_AFTER_SCOPE      0xF8

/****************************************************************************
 * Public Types
 ****************************************************************************/

/**
 * @brief
 *  Describes global variable
 *
 * @note
 *  Layout dictated by compiler
 *
 */
struct kasan_global {
	const void *beg;		/**< Address of the beginning of the global variable. */

	size_t size;			/**< Size of the global variable. */
	size_t size_with_redzone;	/**< Size of the variable + size of the red zone. 32 bytes aligned */
	const void *name;
	const void *module_name;	/**< Name of the module where the global variable is declared. */
	unsigned long has_dynamic_init;	/**< This needed for C++ */
#if KASAN_ABI_VERSION >= 4
	const struct kasan_source_location *location; /**< location of error */
#endif
#if KASAN_ABI_VERSION >= 5
	char *odr_indicator;
#endif
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/**
 * @brief
 *  Initialize KASan shadow memory by filling it with KASAN_REDZONE byte.
 *
 * @return
 *   None
 *
 */
void kasan_init(void);

/**
 * @brief
 *  Print shadow memory region related to normal memory region [addr, addr + size).
 *
 * @param addr pointer to memory region
 * @param size size of memory region
 *
 * @return
 *   None
 *
 */
void kasan_print_shadow_region(unsigned long addr, size_t size);

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
void kasan_poison(unsigned long addr, size_t size, signed char value);

/**
 * @brief
 *  Unpoison shadow memory related to memory region.
 *
 * @param addr pointer to memory region
 * @param size size of memory region
 *
 * @return
 *   None
 *
 * @attention
 *  Memory address should be aligned to KASAN_SHADOW_SCALE_SIZE.
 *
 */
void kasan_unpoison(unsigned long addr, size_t size);

/**
 * @brief
 *  Poison shadow memory related to heap memory region.
 *
 * @param addr pointer to memory
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
void kasan_poison_heap(unsigned long addr, size_t size, signed char value);

/**
 * @brief
 *  Unpoison shadow memory related to heap memory region.
 *
 * @param addr pointer to memory
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
void kasan_unpoison_heap(unsigned long addr, size_t size);

#ifdef CONFIG_KASAN_GLOBALS
/**
 * @brief
 *  Get first and last addresses of data section. This needed for correct work of
 *  UNDER_SHADOW macro.
 *
 * @param start first address of data section
 * @param end last address of data section
 *
 * @return
 *   None
 *
 * @note
 *  Useful only with CONFIG_KASAN_GLOBALS option.
 *
 */
void kasan_set_databounds(unsigned long start, unsigned long end);

/**
 * @brief
 *  Get first and last addresses of bss section. This needed for correct work of
 *  UNDER_SHADOW macro.
 *
 * @param start first address of bss section
 * @param end last address of bss section
 *
 * @return
 *   None
 *
 * @note
 *  Useful only with CONFIG_KASAN_GLOBALS option.
 *
 */
void kasan_set_bssbounds(unsigned long start, unsigned long end);
#else
#define kasan_set_databounds(a, b)
#define kasan_set_bssbounds(a, b)
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
void kasan_set_idlestackbounds(unsigned long start, unsigned long end);
#else
#define kasan_set_idlestackbounds(a, b)
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
void kasan_set_heapbounds(unsigned long start, unsigned long end);
#else
#define kasan_set_heapbounds(a, b)
#endif	/* CONFIG_KASAN_HEAP */

#ifdef CONFIG_KASAN_QRNT
/**
 * @brief
 *  Initialize KASan quarantine queue.
 *
 * @param heapsize size of the heap for watermark calculations
 *
 * @return
 *   None
 *
 */
void kasan_qrnt_init(unsigned long heapsize);

/**
 * @brief
 *  Enqueue entry to quarantine queue
 *
 * @param entry pointer to memory chunk (behind mm_allocnode_s structure)
 *
 * @return
 *   None
 *
 */
void kasan_qrnt_enq(void *entry);

/**
 * @brief
 *  Dequeue entry from quarantine queue.
 *
 * @param
 *   None
 *
 * @return
 *   Pointer to memory chunk.
 *
 */
void *kasan_qrnt_deq(void);

/**
 * @brief
 *   If quarantine size exceeded watermark_max, return memory chunks to
 *   allocator until quarantine size is less than or equal watermark_low.
 *
 * @param
 *   None
 *
 * @return
 *   None
 *
 */
void kasan_qrnt_purge(void);

#else  /* CONFIG_KASAN_QRNT */

#define kasan_qrnt_init(a)
#define kasan_qrnt_enq(a)
#define kasan_qrnt_deq()
#define kasan_qrnt_purge()

#endif	/* CONFIG_KASAN_QRNT */

#else	/* CONFIG_KASAN */

#define KASAN_SHADOW_START	  CONFIG_RAM_END

#define kasan_init()
#define kasan_print_shadow_region(a, b)
#define kasan_poison(a, b, c)
#define kasan_unpoison(a, b)
#define kasan_poison_heap(a, b, c)
#define kasan_unpoison_heap(a, b)
#define kasan_set_databounds(a, b)
#define kasan_set_bssbounds(a, b)
#define kasan_set_idlestackbounds(a, b)
#define kasan_set_heapbounds(a, b)
#define kasan_qrnt_init(a)
#define kasan_qrnt_enq(a)
#define kasan_qrnt_deq()
#define kasan_qrnt_purge()

#endif	/* CONFIG_KASAN */
#endif	/* __MM_KASAN_KASAN_H */
