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
 *
 *   Copyright (C) 2007-2009, 2013-2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __INCLUDE_MM_MM_H
#define __INCLUDE_MM_MM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdbool.h>
#include <semaphore.h>
#include <debug.h>
#include <stdint.h>
#include <tinyara/mm/heap_regioninfo.h>
#ifdef CONFIG_HEAPINFO_USER_GROUP
#include <tinyara/mm/heapinfo_internal.h>
#endif

#include <tinyara/sched.h>
/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/
/* If the MCU has a small (16-bit) address capability, then we will use
 * a smaller chunk header that contains 16-bit size/offset information.
 * We will also use the smaller header on MCUs with wider addresses if
 * CONFIG_MM_SMALL is selected.  This configuration is common with MCUs
 * that have a large FLASH space, but only a tiny internal SRAM.
 */

#ifdef CONFIG_SMALL_MEMORY
/* If the MCU has a small addressing capability, then for the smaller
 * chunk header.
 */

#undef  CONFIG_MM_SMALL
#define CONFIG_MM_SMALL 1
#endif

/* Terminology:
 *
 * - Flat Build: In the flat build (CONFIG_BUILD_FLAT=y), there is only a
 *   single heap access with the standard allocations (malloc/free).  This
 *   heap is referred to as the user heap.  The kernel logic must
 *   initialize this single heap at boot time.
 * - Kernel Build: If the architecture has an MMU, then it may support the
 *   kernel build (CONFIG_BUILD_KERNEL=y).  In this configuration, there
 *   is one kernel heap but multiple user heaps:  One per task group.
 *   However, in this case, the kernel need only be concerned about
 *   initializing the single kernel heap here.  User heaps will be created
 *   as tasks are created.
 *
 */

/* The kernel heap is never accessible from user code */

#if defined(CONFIG_BUILD_PROTECTED) && !defined(__KERNEL__)
#undef CONFIG_MM_KERNEL_HEAP
#endif

/* Chunk Header Definitions *************************************************/
/* These definitions define the characteristics of allocator
 *
 * MM_MIN_SHIFT is used to define MM_MIN_CHUNK.
 * MM_MIN_CHUNK - is the smallest physical chunk that can be allocated.  It
 *   must be at least a large as sizeof(struct mm_freenode_s).  Larger values
 *   may improve performance slightly, but will waste memory due to
 *   quantization losses.
 *
 * MM_MAX_SHIFT is used to define MM_MAX_CHUNK
 * MM_MAX_CHUNK is the largest, contiguous chunk of memory that can be
 *   allocated.  It can range from 16-bytes to 4Gb.  Larger values of
 *   MM_MAX_SHIFT can cause larger data structure sizes and, perhaps,
 *   minor performance losses.
 */

#if defined(CONFIG_MM_SMALL) && UINTPTR_MAX <= UINT32_MAX
/* Two byte offsets; Pointers may be 2 or 4 bytes;
 * sizeof(struct mm_freenode_s) is 8 or 12 bytes.
 * REVISIT: We could do better on machines with 16-bit addressing.
 */

#define MM_MIN_SHIFT    4		/* 16 bytes */
#define MM_MAX_SHIFT   15		/* 32 Kb */
#define MM_SHIFT_FOR_NDX (MM_MIN_SHIFT + 1)
#define MM_SHIFT_MASK    ~(1 << MM_SHIFT_FOR_NDX)

#elif defined(CONFIG_HAVE_LONG_LONG)
/* Four byte offsets; Pointers may be 4 or 8 bytes
 * sizeof(struct mm_freenode_s) is 16 or 24 bytes.
 */

#if UINTPTR_MAX <= UINT32_MAX
#define MM_MIN_SHIFT  4			/* 16 bytes */
#define MM_SHIFT_FOR_NDX 5		/* MM_MIN_SHIFT + 1 */
#define MM_SHIFT_MASK    0xffffffe0	/* ~(1 << MM_SHIFT_FOR_NDX) */
#elif UINTPTR_MAX <= UINT64_MAX
#define MM_MIN_SHIFT  5			/* 32 bytes */
#define MM_SHIFT_FOR_NDX 6		/* MM_MIN_SHIFT + 1 */
#define MM_SHIFT_MASK    0xffffffc0	/* ~(1 << MM_SHIFT_FOR_NDX) */
#endif
#define MM_MAX_SHIFT   22		/*  4 Mb */

#else
/* Four byte offsets; Pointers must be 4 bytes.
 * sizeof(struct mm_freenode_s) is 16 bytes.
 */

#define MM_MIN_SHIFT    4		/* 16 bytes */
#define MM_MAX_SHIFT   22		/*  4 Mb */
#define MM_SHIFT_FOR_NDX 5		/* MM_MIN_SHIFT + 1 */
#define MM_SHIFT_MASK    0xffffffe0	/* ~(1 << MM_SHIFT_FOR_NDX) */
#endif

/* All other definitions derive from these two */

#define MM_MIN_CHUNK     (1 << MM_MIN_SHIFT)
#define MM_MAX_CHUNK     (1 << MM_MAX_SHIFT)
#define MM_NNODES        (MM_MAX_SHIFT - MM_MIN_SHIFT + 1)

#define MM_GRAN_MASK     (MM_MIN_CHUNK-1)
#define MM_ALIGN_UP(a)   (((a) + MM_GRAN_MASK) & ~MM_GRAN_MASK)
#define MM_ALIGN_DOWN(a) ((a) & ~MM_GRAN_MASK)

/* An allocated chunk is distinguished from a free chunk by bit 31 (or 15)
 * of the 'preceding' chunk size.  If set, then this is an allocated chunk.
 */

#ifdef CONFIG_MM_SMALL
#define MM_ALLOC_BIT    0x8000
#else
#define MM_ALLOC_BIT    0x80000000
#endif
#define MM_IS_ALLOCATED(n) ((int)((struct mm_allocnode_s*)(n)->preceding) < 0)

/****************************************************************************
 * Public Types
 ****************************************************************************/

#define HEAPINFO_SIMPLE 1
#define HEAPINFO_DETAIL_ALL 2
#define HEAPINFO_DETAIL_PID 3
#define HEAPINFO_DETAIL_FREE 4
#define HEAPINFO_DETAIL_SPECIFIC_HEAP 5
#define HEAPINFO_INIT_PEAK 6
#define HEAPINFO_DUMP_HEAP 7
#define HEAPINFO_PID_ALL -1

#define HEAPINFO_INIT_INFO -1
#define HEAPINFO_ADD_INFO 1
#define HEAPINFO_DEL_INFO 2

#define HEAPINFO_INVALID_GROUPID -1

#define HEAPINFO_HEAP_TYPE_KERNEL 1
#ifdef CONFIG_APP_BINARY_SEPARATION
#define HEAPINFO_HEAP_TYPE_BINARY    2
#endif

#define INVALID_HEAP_IDX -1

#ifdef CONFIG_MM_KERNEL_HEAP
#define KREGION_START (size_t)kregionx_start[0]
#define KREGION_SIZE  kregionx_size[0]
#define KREGION_END (KREGION_START + KREGION_SIZE)
#endif

#if defined(CONFIG_BUILD_FLAT) || defined(__KERNEL__)
extern bool abort_mode;
#endif

/* Determines the size of the chunk size/offset type */

#ifdef CONFIG_MM_SMALL
typedef uint16_t mmsize_t;
#define MMSIZE_MAX 0xffff
#else
typedef size_t mmsize_t;
#define MMSIZE_MAX SIZE_MAX
#endif

#if defined(CONFIG_ARCH_MIPS)
/* Macro gets return address of malloc API. */
#define ARCH_GET_RET_ADDRESS(caller_retaddr) \
	do { \
		asm volatile ("sw $ra, %0" : "=m" (caller_retaddr)); \
	} while (0);
#elif defined(CONFIG_ARCH_ARM)
/* This macro should be placed at the first of the function.
 * If no, it is better to use "__builtin_return_address(0)" instead.
 */
#define ARCH_GET_RET_ADDRESS(caller_retaddr) \
	do { \
		asm volatile ("mov %0,lr\n" : "=r" (caller_retaddr));\
	} while (0);
#elif defined(CONFIG_ARCH_XTENSA)
#define ARCH_GET_RET_ADDRESS(caller_retaddr) \
	do { \
		asm volatile ("mov %0,a0\n" : "=&r" (caller_retaddr));\
	} while (0);
#else
#error Unknown CONFIG_ARCH option, malloc debug feature wont work.
#endif

#ifdef CONFIG_DEBUG_MM_HEAPINFO
/* This macro updates a caller return address in the 'mem' node.
 * This will show the real owner of the 'mem' even it's allocated through wrapping APIs of malloc.
 */
#define DEBUG_SET_CALLER_ADDR(mem) heapinfo_set_caller_addr(mem, __builtin_return_address(0))
#else
#define DEBUG_SET_CALLER_ADDR(mem)
#endif

/* typedef is used for defining size of address space */

#ifdef CONFIG_DEBUG_MM_HEAPINFO
typedef size_t mmaddress_t;             /* 32 bit address space */

#define SIZEOF_MM_MALLOC_DEBUG_INFO \
	(sizeof(mmaddress_t) + sizeof(pid_t) + sizeof(uint16_t))
#endif

/* This describes an allocated chunk.  An allocated chunk is
 * distinguished from a free chunk by bit 15/31 of the 'preceding' chunk
 * size.  If set, then this is an allocated chunk.
 */

struct mm_allocnode_s {
	mmsize_t preceding;				/* Size of the preceding chunk */
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	mmaddress_t alloc_call_addr;			/* malloc call address */
	pid_t pid;					/* PID info */
	uint16_t reserved;				/* Reserved for future use. */
#endif
	mmsize_t size;					/* Size of this chunk */

};

/* What is the size of the allocnode? */

#ifdef CONFIG_MM_SMALL
#ifdef CONFIG_DEBUG_MM_HEAPINFO
/* 10 = (uint16_t + uint16_t + uint16_t + uint16_t + uint16_t ) */
#define SIZEOF_MM_ALLOCNODE   (sizeof(mmsize_t) + sizeof(mmsize_t) + SIZEOF_MM_MALLOC_DEBUG_INFO)
#else
/* 4 = (uint16_t + uint16_t) */
#define SIZEOF_MM_ALLOCNODE   (sizeof(mmsize_t) + sizeof(mmsize_t))
#endif

#else

#ifdef CONFIG_DEBUG_MM_HEAPINFO
/* 16 = (uint32_t + uint32_t + uint32_t + uint16_t + uint16_t ) */
#define SIZEOF_MM_ALLOCNODE  (sizeof(mmsize_t) + sizeof(mmsize_t) + SIZEOF_MM_MALLOC_DEBUG_INFO)
#else
/* 8 = (uint32_t + uint32_t) */
#define SIZEOF_MM_ALLOCNODE   (sizeof(mmsize_t) + sizeof(mmsize_t))
#endif
#endif

#define CHECK_ALLOCNODE_SIZE \
	DEBUGASSERT(sizeof(struct mm_allocnode_s) == SIZEOF_MM_ALLOCNODE)

/* This describes a free chunk */

struct mm_freenode_s {
	mmsize_t preceding;			/* Size of the preceding chunk */
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	mmaddress_t alloc_call_addr;			/* malloc call address */
	pid_t pid;					/* PID info */
	uint16_t reserved;				/* Reserved for future use. */
#endif
	mmsize_t size;				/* Size of this chunk */
	FAR struct mm_freenode_s *flink;	/* Supports a doubly linked list */
	FAR struct mm_freenode_s *blink;
};

/* What is the size of the freenode? */

#define MM_PTR_SIZE sizeof(FAR struct mm_freenode_s *)
#define SIZEOF_MM_FREENODE (SIZEOF_MM_ALLOCNODE + 2 * MM_PTR_SIZE)

#define CHECK_FREENODE_SIZE \
	DEBUGASSERT(sizeof(struct mm_freenode_s) == SIZEOF_MM_FREENODE)

#ifdef CONFIG_DEBUG_MM_HEAPINFO
struct heapinfo_tcb_info_s {
	int pid;
	int curr_alloc_size;
	int peak_alloc_size;
	int num_alloc_free;
};
typedef struct heapinfo_tcb_info_s heapinfo_tcb_info_t;
#ifdef CONFIG_HEAPINFO_USER_GROUP
struct heapinfo_group_info_s {
	int pid;
	int group;
	int stack_size;
};

struct heapinfo_group_s {
	int curr_size;
	int peak_size;
	int stack_size;
	int heap_size;
};

#endif
#ifdef CONFIG_HEAPINFO_USER_GROUP
extern int heapinfo_max_group;
extern struct heapinfo_group_s heapinfo_group[HEAPINFO_USER_GROUP_NUM];
extern struct heapinfo_group_info_s group_info[HEAPINFO_THREAD_NUM];
#endif

#endif

struct mm_alloc_fail_s {
	uint32_t size;
	uint32_t caller;
};

/* This describes one heap (possibly with multiple regions) */

struct mm_heap_s {
	/* Mutually exclusive access to this data set is enforced with
	 * the following un-named semaphore.
	 */

	sem_t mm_semaphore;
	pid_t mm_holder;
	int mm_counts_held;

	/* This is the size of the heap provided to mm */

	size_t mm_heapsize;
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	size_t peak_alloc_size;
	size_t total_alloc_size;
#ifdef CONFIG_HEAPINFO_USER_GROUP
	int max_group;
	struct heapinfo_group_s group[HEAPINFO_USER_GROUP_NUM];
#endif
	/* Linked List for heap information per pid */
	heapinfo_tcb_info_t alloc_list[CONFIG_MAX_TASKS];
#endif

	/* This is the first and last nodes of the heap */
	FAR struct mm_allocnode_s *mm_heapstart[CONFIG_KMM_REGIONS];
	FAR struct mm_allocnode_s *mm_heapend[CONFIG_KMM_REGIONS];

#if CONFIG_KMM_REGIONS > 1
	int mm_nregions;
#endif

	/* All free nodes are maintained in a doubly linked list.  This
	 * array provides some hooks into the list at various points to
	 * speed searches for free nodes.
	 */

	struct mm_freenode_s mm_nodelist[MM_NNODES + 1];
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

extern struct mm_heap_s g_kmmheap[CONFIG_KMM_NHEAPS];
#if defined(CONFIG_ARCH_ADDRENV) && defined(CONFIG_BUILD_KERNEL)
/* In the kernel build, there are multiple user heaps; one for each task
 * group.  In this build configuration, the user heap structure lies
 * in a reserved region at the beginning of the .bss/.data address
 * space (CONFIG_ARCH_DATA_VBASE).  The size of that region is given by
 * ARCH_DATA_RESERVE_SIZE
 */
#include <tinyara/addrenv.h>
#define BASE_HEAP (&ARCH_DATA_RESERVE->ar_usrheap)

#elif defined(CONFIG_BUILD_PROTECTED) && defined(__KERNEL__)
#include <tinyara/sched.h>
#define BASE_HEAP ((struct mm_heap_s *)((struct tcb_s*)sched_self())->uheap)

#elif defined(CONFIG_BUILD_PROTECTED) && !defined(__KERNEL__)
extern uint32_t _sdata;

#ifdef CONFIG_SUPPORT_COMMON_BINARY
extern struct mm_heap_s *g_app_heap_table[CONFIG_NUM_APPS + 1];
extern uint32_t g_cur_app;
#define BASE_HEAP (g_app_heap_table[g_cur_app])

#else
#define BASE_HEAP ((struct mm_heap_s *)_sdata)
#endif

#else
#define BASE_HEAP       g_kmmheap
#endif

// Check if the kernel heap has been locked by any process.
#define IS_KMM_LOCKED()		(g_kmmheap->mm_counts_held > 0 ? 1 : 0)

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/* Functions contained in mm_initialize.c ***********************************/

int mm_initialize(FAR struct mm_heap_s *heap, FAR void *heap_start, size_t heap_size);
int mm_addregion(FAR struct mm_heap_s *heap, FAR void *heapstart, size_t heapsize);

/* Functions contained in umm_initialize.c **********************************/


/* Functions contained in kmm_initialize.c **********************************/

#ifdef CONFIG_MM_KERNEL_HEAP
int kmm_initialize(FAR void *heap_start, size_t heap_size);
#endif

/* Functions contained in umm_addregion.c ***********************************/

#if !defined(CONFIG_BUILD_PROTECTED) || !defined(__KERNEL__)
int umm_initialize(FAR void *heap_start, size_t heap_size);
int umm_addregion(FAR void *heapstart, size_t heapsize);
#endif

/* Functions contained in kmm_addregion.c ***********************************/

#ifdef CONFIG_MM_KERNEL_HEAP
int kmm_addregion(FAR void *heapstart, size_t heapsize);
#endif

/* Functions contained in mm_sem.c ******************************************/

void mm_seminitialize(FAR struct mm_heap_s *heap);
void mm_takesemaphore(FAR struct mm_heap_s *heap);
int mm_trysemaphore(FAR struct mm_heap_s *heap);
void mm_givesemaphore(FAR struct mm_heap_s *heap);

/* Functions contained in umm_sem.c ****************************************/

#if !defined(CONFIG_BUILD_PROTECTED) || !defined(__KERNEL__)
int umm_trysemaphore(void *address);
void umm_givesemaphore(void *address);
#endif

/* Functions contained in kmm_sem.c ****************************************/

#ifdef CONFIG_MM_KERNEL_HEAP
int kmm_trysemaphore(void *address);
void kmm_givesemaphore(void *address);
#endif
#ifdef CONFIG_DEBUG_MM_HEAPINFO

/* Functions contained in mm_malloc.c ***************************************/

FAR void *mm_malloc(FAR struct mm_heap_s *heap, size_t size, mmaddress_t caller_retaddr);

#else

/* Functions contained in mm_malloc.c ***************************************/

FAR void *mm_malloc(FAR struct mm_heap_s *heap, size_t size);
#endif

/* Functions contained in kmm_malloc.c **************************************/

#ifdef CONFIG_MM_KERNEL_HEAP
FAR void *kmm_malloc(size_t size);
#endif

/* Functions contained in mm_free.c *****************************************/

void mm_free(FAR struct mm_heap_s *heap, FAR void *mem);

/* Functions contained in kmm_free.c ****************************************/

#ifdef CONFIG_MM_KERNEL_HEAP
void kmm_free(FAR void *mem);
#endif

#ifdef CONFIG_DEBUG_MM_HEAPINFO

/* Functions contained in mm_realloc.c **************************************/

FAR void *mm_realloc(FAR struct mm_heap_s *heap, FAR void *oldmem, size_t size, mmaddress_t caller_retaddr);

#else

/* Functions contained in mm_realloc.c **************************************/

FAR void *mm_realloc(FAR struct mm_heap_s *heap, FAR void *oldmem, size_t size);
#endif

/* Functions contained in kmm_realloc.c *************************************/

#ifdef CONFIG_MM_KERNEL_HEAP
FAR void *kmm_realloc(FAR void *oldmem, size_t newsize);
#endif
#ifdef CONFIG_DEBUG_MM_HEAPINFO

/* Functions contained in mm_calloc.c ***************************************/

FAR void *mm_calloc(FAR struct mm_heap_s *heap, size_t n, size_t elem_size, mmaddress_t caller_retaddr);

#else

/* Functions contained in mm_calloc.c ***************************************/

FAR void *mm_calloc(FAR struct mm_heap_s *heap, size_t n, size_t elem_size);
#endif

/* Functions contained in kmm_calloc.c **************************************/

#ifdef CONFIG_MM_KERNEL_HEAP
FAR void *kmm_calloc(size_t n, size_t elem_size);
#endif

#ifdef CONFIG_DEBUG_MM_HEAPINFO
/* Functions contained in mm_zalloc.c ***************************************/

FAR void *mm_zalloc(FAR struct mm_heap_s *heap, size_t size, mmaddress_t caller_retaddr);
#else
/* Functions contained in mm_zalloc.c ***************************************/

FAR void *mm_zalloc(FAR struct mm_heap_s *heap, size_t size);
#endif

/* Functions contained in kmm_zalloc.c **************************************/

#ifdef CONFIG_MM_KERNEL_HEAP
FAR void *kmm_zalloc(size_t size);
#endif
#ifdef CONFIG_DEBUG_MM_HEAPINFO
/* Functions contained in mm_memalign.c *************************************/

FAR void *mm_memalign(FAR struct mm_heap_s *heap, size_t alignment, size_t size, mmaddress_t caller_retaddr);

#else

/* Functions contained in mm_memalign.c *************************************/

FAR void *mm_memalign(FAR struct mm_heap_s *heap, size_t alignment, size_t size);
#endif

/* Functions contained in kmm_memalign.c ************************************/

#ifdef CONFIG_MM_KERNEL_HEAP
FAR void *kmm_memalign(size_t alignment, size_t size);
#endif

/* Functions contained in kmm_heapmember.c **********************************/

#if defined(CONFIG_MM_KERNEL_HEAP)
bool kmm_heapmember(FAR void *mem);
#endif

/* Functions contained in mm_brkaddr.c **************************************/

FAR void *mm_brkaddr(FAR struct mm_heap_s *heap, int region);

/* Functions contained in umm_brkaddr.c *************************************/

#if !defined(CONFIG_BUILD_PROTECTED) || !defined(__KERNEL__)
FAR void *umm_brkaddr(int region);
#endif

/* Functions contained in kmm_brkaddr.c *************************************/

#ifdef CONFIG_MM_KERNEL_HEAP
FAR void *kmm_brkaddr(int region);
#endif

/* Functions contained in mm_sbrk.c *****************************************/

#if defined(CONFIG_ARCH_ADDRENV) && defined(CONFIG_MM_PGALLOC) && \
	defined(CONFIG_ARCH_USE_MMU)
FAR void *mm_sbrk(FAR struct mm_heap_s *heap, intptr_t incr, uintptr_t maxbreak);
#endif

/* Functions contained in kmm_sbrk.c ****************************************/

#if defined(CONFIG_MM_KERNEL_HEAP) && defined(CONFIG_ARCH_ADDRENV) && \
	defined(CONFIG_MM_PGALLOC) && defined(CONFIG_ARCH_USE_MMU)
FAR void *kmm_sbrk(intptr_t incr);
#endif

/* Functions contained in mm_extend.c ***************************************/

void mm_extend(FAR struct mm_heap_s *heap, FAR void *mem, size_t size, int region);

/* Functions contained in umm_extend.c **************************************/

#if !defined(CONFIG_BUILD_PROTECTED) || !defined(__KERNEL__)
void umm_extend(FAR void *mem, size_t size, int region);
#endif

/* Functions contained in kmm_extend.c **************************************/

#ifdef CONFIG_MM_KERNEL_HEAP
void kmm_extend(FAR void *mem, size_t size, int region);
#endif

/* Functions contained in mm_mallinfo.c *************************************/

struct mallinfo;				/* Forward reference */
int mm_mallinfo(FAR struct mm_heap_s *heap, FAR struct mallinfo *info);

/* Functions contained in kmm_mallinfo.c ************************************/

#ifdef CONFIG_MM_KERNEL_HEAP
#ifdef CONFIG_CAN_PASS_STRUCTS
struct mallinfo kmm_mallinfo(void);
#else
int kmm_mallinfo(struct mallinfo *info);
#endif
#endif							/* CONFIG_CAN_PASS_STRUCTS */

#ifdef CONFIG_MM_KERNEL_HEAP
struct mm_heap_s *kmm_get_baseheap(void);
#endif

/* Functions contained in mm_shrinkchunk.c **********************************/

void mm_shrinkchunk(FAR struct mm_heap_s *heap, FAR struct mm_allocnode_s *node, size_t size);

/* Functions contained in mm_addfreechunk.c *********************************/

void mm_addfreechunk(FAR struct mm_heap_s *heap, FAR struct mm_freenode_s *node);

/* Functions contained in mm_size2ndx.c.c ***********************************/

int mm_size2ndx(size_t size);

void mm_dump_heap_region(uint32_t start, uint32_t end);
int heap_dbg(const char *fmt, ...);
#ifdef CONFIG_DEBUG_MM_HEAPINFO
/* Functions contained in kmm_mallinfo.c . Used to display memory allocation details */
void heapinfo_parse_heap(FAR struct mm_heap_s *heap, int mode, pid_t pid);
/* Funciton to add memory allocation info */
void heapinfo_update_node(FAR struct mm_allocnode_s *node, mmaddress_t caller_retaddr);
void heapinfo_set_caller_addr(void *address, mmaddress_t caller_retaddr);

void heapinfo_add_size(struct mm_heap_s *heap, pid_t pid, mmsize_t size);
void heapinfo_subtract_size(struct mm_heap_s *heap, pid_t pid, mmsize_t size);
void heapinfo_update_total_size(struct mm_heap_s *heap, mmsize_t size, pid_t pid);
void heapinfo_set_stack_node(void *stack_ptr, pid_t pid);
void heapinfo_exclude_stacksize(void *stack_ptr);
void heapinfo_peak_init(struct mm_heap_s *heap);
void heapinfo_dealloc_tcbinfo(void *address, pid_t pid);
void heapinfo_dump_heap(struct mm_heap_s *heap);
#ifdef CONFIG_HEAPINFO_USER_GROUP
void heapinfo_update_group(mmsize_t size, pid_t pid);
void heapinfo_update_group_info(pid_t pid, int group, int type);
void heapinfo_check_group_list(pid_t pid, char *name);
#endif
#endif
void mm_is_sem_available(void *address);

/* Functions to get the address of heap structure */
struct mm_heap_s *mm_get_heap(void *address);
struct mm_heap_s *mm_get_heap_with_index(int index);

int mm_get_index_of_heap(void *mem);
size_t mm_get_largest_freenode_size(void);
#ifdef CONFIG_DEBUG_MM_HEAPINFO
size_t mm_get_heap_free_size(void);
#endif

#if defined(CONFIG_APP_BINARY_SEPARATION) && defined(__KERNEL__)
void mm_initialize_app_heap_q(void);
void mm_add_app_heap_list(struct mm_heap_s *heap, char *app_name);
void mm_remove_app_heap_list(struct mm_heap_s *heap);
void mm_disable_app_heap_list(struct mm_heap_s *heap);
struct mm_heap_s *mm_get_app_heap_with_name(char *app_name);
char *mm_get_app_heap_name(void *address);
#endif

struct mm_heap_s *umm_get_heap(void *address);
struct mm_heap_s *kmm_get_heap(void *addr);
struct mm_heap_s *kmm_get_heap_with_index(int index);
int kmm_get_index_of_heap(void *mem);
size_t kmm_get_largest_freenode_size(void);
size_t umm_get_largest_freenode_size(void);
#ifdef CONFIG_DEBUG_MM_HEAPINFO
size_t kmm_get_heap_free_size(void);
size_t umm_get_heap_free_size(void);
#endif

/* Function to check heap corruption */
int mm_check_heap_corruption(struct mm_heap_s *heap);

#define USER_HEAP   1
#define KERNEL_HEAP 2

#define HEAP_START_IDX 0
#define HEAP_END_IDX   (CONFIG_KMM_NHEAPS - 1)

/* Function to manage the memory allocation failure case. */
#if defined(CONFIG_APP_BINARY_SEPARATION) && !defined(__KERNEL__)
#ifdef CONFIG_DEBUG_MM_HEAPINFO
void mm_ioctl_alloc_fail(size_t size, uint32_t caller);
#define mm_manage_alloc_fail(h, b, e, s, t, c) 	do { \
							(void)h; \
							(void)b; \
							(void)e; \
							(void)t; \
							mm_ioctl_alloc_fail(s, c); \
						} while (0)
#else
void mm_ioctl_alloc_fail(size_t size);
#define mm_manage_alloc_fail(h, b, e, s, t) 	do { \
							(void)h; \
							(void)b; \
							(void)e; \
							(void)t; \
							mm_ioctl_alloc_fail(s); \
						} while (0)
#endif
#else
void mm_manage_alloc_fail(struct mm_heap_s *heap, int startidx, int endidx, size_t size, int heap_type
#ifdef CONFIG_DEBUG_MM_HEAPINFO
		, uint32_t caller
#endif
		);
#endif

#if CONFIG_KMM_NHEAPS > 1
/**
 * @cond
 * @internal
 */
/**
 * @brief Allocate memory to the specific heap.
 * @details @b #include <tinyara/mm/mm.h>\n
 *   malloc_at tries to allocate memory for a specific heap which passed by api argument.
 *   If there is no enough space to allocate, it will return NULL.
 * @param[in] heap_index Index of specific heap
 * @param[in] size size (in bytes) of the memory region to be allocated
 * 
 * @return On success, the address of the allocated memory is returned. On failure, NULL is returned.
 * @since TizenRT v2.1 PRE
 */
void *malloc_at(int heap_index, size_t size);
/**
 * @brief Calloc to the specific heap.
 * @details @b #include <tinyara/mm/mm.h>\n
 *   calloc_at tries to allocate memory for a specific heap which passed by api argument.
 *   If there is no enough space to allocate, it will return NULL.
 * @param[in] heap_index Index of specific heap
 * @param[in] n the number of elements to be allocated
 * @param[in] elem_size the size of elements
 * 
 * @return On success, the address of the allocated memory is returned. On failure, NULL is returned.
 * @since TizenRT v2.1 PRE
 */
void *calloc_at(int heap_index, size_t n, size_t elem_size);
/**
 * @brief Memalign to the specific heap.
 * @details @b #include <tinyara/mm/mm.h>\n
 *   memalign_at tries to align the memory for a specific heap which passed by api argument.
 *   If there is no enough space, it will return NULL.
 * @param[in] heap_index Index of specific heap
 * @param[in] alignment A power of two for alignment
 * @param[in] size Allocated memory size
 * 
 * @return On success, the address of the allocated memory is returned. On failure, NULL is returned.
 * @since TizenRT v2.1 PRE
 */
void *memalign_at(int heap_index, size_t alignment, size_t size);
/**
 * @brief Realloc to the specific heap.
 * @details @b #include <tinyara/mm/mm.h>\n
 *   realloc_at tries to allocate memory for a specific heap which passed by api argument.
 *   If there is no enough space to allocate, it will return NULL.
 * @param[in] heap_index Index of specific heap
 * @param[in] oldmem the pointer to a memory block previously allocated
 * @param[in] size the new size for the memory block
 * 
 * @return On success, the address of the allocated memory is returned. On failure, NULL is returned.
 * @since TizenRT v2.1 PRE
 */
void *realloc_at(int heap_index, void *oldmem, size_t size);
/**
 * @brief Zalloc to the specific heap.
 * @details @b #include <tinyara/mm/mm.h>\n
 *   zalloc_at tries to allocate memory for a specific heap which passed by api argument.
 *   If there is no enough space to allocate, it will return NULL.
 * @param[in] heap_index Index of specific heap
 * @param[in] size size (in bytes) of the memory region to be allocated
 * 
 * @return On success, the address of the allocated memory is returned. On failure, NULL is returned.
 * @since TizenRT v2.1 PRE
 */
void *zalloc_at(int heap_index, size_t size);
#else
#define malloc_at(heap_index, size)              malloc(size)
#define calloc_at(heap_index, n, elem_size)      calloc(n, elem_size)
#define memalign_at(heap_index, alignment, size) memalign(alignment, size)
#define realloc_at(heap_index, oldmem, size)     realloc(oldmem, size)
#define zalloc_at(heap_index, size)              zalloc(size)
#endif

#ifdef CONFIG_MEM_LEAK_CHECKER
struct mem_leak_checker_info_s {
	void *stack_top;
	void *stack_bottom;
	struct mm_allocnode_s **heap_start;
	struct mm_allocnode_s **heap_end;
	void *heap;
	int regions;
};
int run_all_mem_leak_checker(int checker_pid);
#endif
/**
 * @brief Free the memory from specified user heap.
 * @details @b #include <tinyara/mm/mm.h>\n
 *   This is a simple wrapper for the mm_free() function.
 *   This function is used to free the memory from specified user heap.
 */
void free_user_at(struct mm_heap_s *heap, void *mem);
/**
 * @brief Allocate to specified user heap.
 * @details @b #include <tinyara/mm/mm.h>\n
 *   This is a simple wrapper for the mm_calloc() function.
 *   This function is used to allocate the memory from kernel side
 *   to specified user heap.
 */
void *calloc_user_at(struct mm_heap_s *heap, size_t n, size_t elem_size);
/**
 * @brief Allocate to specified user heap.
 * @details @b #include <tinyara/mm/mm.h>\n
 *   This is a simple wrapper for the mm_memalign() function.
 *   This function is used to allocate the memory from kernel side
 *   to specified user heap.
 */
void *memalign_user_at(struct mm_heap_s *heap, size_t alignment, size_t size);
/**
 * @brief Allocate to specified user heap.
 * @details @b #include <tinyara/mm/mm.h>\n
 *   This is a simple wrapper for the mm_malloc() function.
 *   This function is used to allocate the memory from kernel side
 *   to specified user heap.
 */
void *malloc_user_at(struct mm_heap_s *heap, size_t size);
/**
 * @brief Allocate to specified user heap.
 * @details @b #include <tinyara/mm/mm.h>\n
 *   This is a simple wrapper for the mm_realloc() function.
 *   This function is used to allocate the memory from kernel side
 *   to specified user heap.
 */
void *realloc_user_at(struct mm_heap_s *heap, void *oldmem, size_t newsize);
/**
 * @brief Allocate to specified user heap.
 * @details @b #include <tinyara/mm/mm.h>\n
 *   This is a simple wrapper for the mm_zalloc() function.
 *   This function is used to allocate the memory from kernel side
 *   to specified user heap.
 */
void *zalloc_user_at(struct mm_heap_s *heap, size_t size);

/**
 * @endcond
 */

#ifdef CONFIG_MM_KERNEL_HEAP
#if CONFIG_KMM_NHEAPS > 1
void *kmm_malloc_at(int heap_index, size_t size);
void *kmm_calloc_at(int heap_index, size_t n, size_t elem_size);
void *kmm_memalign_at(int heap_index, size_t alignment, size_t size);
void *kmm_realloc_at(int heap_index, void *oldmem, size_t size);
void *kmm_zalloc_at(int heap_index, size_t size);
#else
#define kmm_malloc_at(heap_index, size)              kmm_malloc(size)
#define kmm_calloc_at(heap_index, n, elem_size)      kmm_calloc(n, elem_size)
#define kmm_memalign_at(heap_index, alignment, size) kmm_memalign(alignment, size)
#define kmm_realloc_at(heap_index, oldmem, size)     kmm_realloc(oldmem, size)
#define kmm_zalloc_at(heap_index, size)              kmm_zalloc(size)
#endif
#endif

static inline uint32_t mm_align_up_by_size(uint32_t address, uint32_t size)
{
	uint32_t align_mask = size - 1;
	return ((address + align_mask) & ~align_mask);
}

static inline uint32_t mm_align_down_by_size(uint32_t address, uint32_t size)
{
	uint32_t align_mask = size - 1;
	return (address & ~align_mask);
}

#undef EXTERN
#ifdef __cplusplus
}

#endif

#endif							/* __INCLUDE_MM_MM_H */
