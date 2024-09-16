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
/****************************************************************************
 * mm/umm_heap/umm_malloc.c
 *
 *   Copyright (C) 2007, 2009, 2013-2014  Gregory Nutt. All rights reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdlib.h>
#include <unistd.h>
#include <debug.h>
#include <tinyara/mm/mm.h>
#include <tinyara/common_logs/common_logs.h>

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

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
#ifndef CONFIG_APP_BINARY_SEPARATION
/************************************************************************
 * Name: malloc_at
 *
 * Description:
 *   Allocate memory to the specific heap.
 *   malloc_at tries to allocate memory for a specific heap which passed by api argument.
 *   If there is no enough space to allocate, it will return NULL.
 *
 * Parameters:
 *   heap_index - Index of specific heap.
 *   size - Size (in bytes) of the memory region to be allocated.
 *
 * Return Value:
 *   The address of the allocated memory (NULL on failure to allocate)
 *
 ************************************************************************/

#if CONFIG_KMM_NHEAPS > 1
void *malloc_at(int heap_index, size_t size)
{
	void *ret;
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	mmaddress_t caller_retaddr = 0;
	ARCH_GET_RET_ADDRESS(caller_retaddr)
#endif
	if (heap_index > HEAP_END_IDX || heap_index < HEAP_START_IDX) {
		mdbg("%s malloc_at. Wrong heap index (%d) of (%d)\n", clog_message_str[CMN_LOG_ALLOC_FAIL], heap_index, HEAP_END_IDX);
		return NULL;
	}

	if (size == 0) {
		return NULL;
	}
	ret = mm_malloc(&BASE_HEAP[heap_index], size
#ifdef CONFIG_DEBUG_MM_HEAPINFO
			, caller_retaddr
#endif
			);
	if (ret == NULL) {
		mm_manage_alloc_fail(&BASE_HEAP[heap_index], heap_index, heap_index, size, USER_HEAP
#ifdef CONFIG_DEBUG_MM_HEAPINFO
				, caller_retaddr
#endif
				);
	}
	return ret;
}
#endif

#ifndef CONFIG_BUILD_KERNEL
/************************************************************************
 * Name: heap_malloc
 *
 * Description:
 *   Traverse the user heap arrays by index, and try to alloc memory.
 *
 * Parameters:
 *   size - Size (in bytes) of the memory region to be allocated.
 *   s     - Start index
 *   e     - End index
 *   caller_retaddr - caller function return address, used only for DEBUG_MM_HEAPINFO
 * Return Value:
 *   The address of the allocated memory (NULL on failure to allocate)
 *
 ************************************************************************/
#ifdef CONFIG_DEBUG_MM_HEAPINFO
static void *heap_malloc(size_t size, int s, int e, mmaddress_t caller_retaddr)
#else
static void *heap_malloc(size_t size, int s, int e)
#endif
{
	int heap_idx;
	void *ret;

	for (heap_idx = s; heap_idx <= e; heap_idx++) {
		ret = mm_malloc(&BASE_HEAP[heap_idx], size
#ifdef CONFIG_DEBUG_MM_HEAPINFO
				, caller_retaddr
#endif
				);
		if (ret != NULL) {
			return ret;
		}
	}

	mm_manage_alloc_fail(BASE_HEAP, s, e, size, USER_HEAP
#ifdef CONFIG_DEBUG_MM_HEAPINFO
			, caller_retaddr
#endif
			);
	return NULL;
}
#endif
#endif

/************************************************************************
 * Name: malloc
 *
 * Description:
 *   Allocate memory from the user heap.
 *
 * Parameters:
 *   size - Size (in bytes) of the memory region to be allocated.
 *
 * Return Value:
 *   The address of the allocated memory (NULL on failure to allocate)
 *
 ************************************************************************/

FAR void *malloc(size_t size)
{
#ifdef CONFIG_BUILD_KERNEL
	FAR void *brkaddr;
	FAR void *mem;

	/* Loop until we successfully allocate the memory or until an error
	 * occurs. If we fail to allocate memory on the first pass, then call
	 * sbrk to extend the heap by one page.  This may require several
	 * passes if more the size of the allocation is more than one page.
	 *
	 * An alternative would be to increase the size of the heap by the
	 * full requested allocation in sbrk().  Then the loop should never
	 * execute more than twice (but more memory than we need may be
	 * allocated).
	 */

	do {
		mem = mm_malloc(BASE_HEAP, size);
		if (!mem) {
			brkaddr = sbrk(size);
			if (brkaddr == (FAR void *)-1) {
				return NULL;
			}
		}
	} while (mem == NULL);

	return mem;
#else /* CONFIG_BUILD_KERNEL */

	void *ret = NULL;

	if (size == 0) {
		return NULL;
	}

#ifdef CONFIG_DEBUG_MM_HEAPINFO
	mmaddress_t caller_retaddr = 0;
	ARCH_GET_RET_ADDRESS(caller_retaddr)
#endif

#ifdef CONFIG_APP_BINARY_SEPARATION
	/* User supports a single heap on app separation */
	ret = mm_malloc(BASE_HEAP, size
#ifdef CONFIG_DEBUG_MM_HEAPINFO
			, caller_retaddr
#endif
			);
	if (ret == NULL) {
		mm_manage_alloc_fail(BASE_HEAP, HEAP_START_IDX, HEAP_END_IDX, size, USER_HEAP
#ifdef CONFIG_DEBUG_MM_HEAPINFO
				, caller_retaddr
#endif
				);
	}

#else /* CONFIG_APP_BINARY_SEPARATION */

	int heap_idx = HEAP_START_IDX;
#ifdef CONFIG_RAM_MALLOC_PRIOR_INDEX
	heap_idx = CONFIG_RAM_MALLOC_PRIOR_INDEX;
#endif

	ret = heap_malloc(size, heap_idx, HEAP_END_IDX
#ifdef CONFIG_DEBUG_MM_HEAPINFO
				, caller_retaddr
#endif
				);
	if (ret != NULL) {
		return ret;
	}

#if (defined(CONFIG_RAM_MALLOC_PRIOR_INDEX) && CONFIG_RAM_MALLOC_PRIOR_INDEX > 0)
	/* Try to mm_calloc to other heaps */
	ret = heap_malloc(size, HEAP_START_IDX, CONFIG_RAM_MALLOC_PRIOR_INDEX - 1
#ifdef CONFIG_DEBUG_MM_HEAPINFO
				, caller_retaddr
#endif
				);
#endif
#endif /* CONFIG_APP_BINARY_SEPARATION */

	return ret;
#endif /* CONFIG_BUILD_KERNEL */
}
