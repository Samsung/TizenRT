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
 * mm/umm/umm_zalloc.c
 *
 *   Copyright (C) 2007, 2009, 2014 Gregory Nutt. All rights reserved.
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
#include <string.h>
#include <debug.h>
#include <tinyara/mm/mm.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
#ifndef CONFIG_APP_BINARY_SEPARATION
/************************************************************************
 * Name: zalloc_at
 *
 * Description:
 *   zalloc to the specific heap.
 *   zalloc_at tries to allocate memory for a specific heap which passed by api argument.
 *   If there is no enough space to allocate, it will return NULL.
 *
 * Return Value:
 *   The address of the allocated memory (NULL on failure to allocate)
 *
 ************************************************************************/

#if CONFIG_KMM_NHEAPS > 1
void *zalloc_at(int heap_index, size_t size)
{
	void *ret;

	mmaddress_t caller_retaddr = NULL;	/* for generalising the call to mm_zalloc api */
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	caller_retaddr = GET_RETURN_ADDRESS();
#endif
	if (heap_index > HEAP_END_IDX || heap_index < HEAP_START_IDX) {
		mdbg("zalloc_at failed. Wrong heap index (%d) of (%d)\n", heap_index, HEAP_END_IDX);
		return NULL;
	}

	if (size == 0) {
		return NULL;
	}
	ret = mm_zalloc(&BASE_HEAP[heap_index], size, caller_retaddr);
	if (ret == NULL) {
		mm_manage_alloc_fail(&BASE_HEAP[heap_index], heap_index, heap_index, size, 0, USER_HEAP, caller_retaddr);
	}
	return ret;
}
#endif

#ifndef CONFIG_ARCH_ADDRENV
/************************************************************************
 * Name: heap_zalloc
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

static void *heap_zalloc(size_t size, int s, int e, mmaddress_t caller_retaddr)
{
	int heap_idx;
	void *ret;

	for (heap_idx = s; heap_idx <= e; heap_idx++) {
		ret = mm_zalloc(&BASE_HEAP[heap_idx], size, caller_retaddr);
		if (ret != NULL) {
			return ret;
		}
	}

	mm_manage_alloc_fail(BASE_HEAP, s, e, size, 0, USER_HEAP, caller_retaddr);
	return NULL;
}
#endif
#endif

/************************************************************************
 * Name: zalloc
 *
 * Description:
 *   Allocate and zero memory from the user heap.
 *
 * Parameters:
 *   size - Size (in bytes) of the memory region to be allocated.
 *
 * Return Value:
 *   The address of the allocated memory (NULL on failure to allocate)
 *
 ************************************************************************/

FAR void *zalloc(size_t size)
{
	if (size == 0) {
		return NULL;
	}

	mmaddress_t caller_retaddr = NULL;	/* for generalising the call to mm_zalloc api */
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	caller_retaddr = GET_RETURN_ADDRESS();
#endif

#ifdef CONFIG_ARCH_ADDRENV
	/* Use malloc() because it implements the sbrk() logic */

	FAR void *alloc = malloc(size);
	if (alloc) {
		memset(alloc, 0, size);
	} else {
		mm_manage_alloc_fail(BASE_HEAP, HEAP_START_IDX, HEAP_END_IDX, size, 0, USER_HEAP, caller_retaddr);
	}

	return alloc;

#else /* CONFIG_ARCH_ADDRENV */
	/* Use mm_zalloc() becuase it implements the clear */
	void *ret = NULL;

#ifdef CONFIG_APP_BINARY_SEPARATION
	/* User supports a single heap on app separation */
	ret = mm_zalloc(BASE_HEAP, size, caller_retaddr);
	if (ret == NULL) {
		mm_manage_alloc_fail(BASE_HEAP, HEAP_START_IDX, HEAP_END_IDX, size, 0, USER_HEAP, caller_retaddr);
	}

#else /* CONFIG_APP_BINARY_SEPARATION */

	int heap_idx = HEAP_START_IDX;
#ifdef CONFIG_RAM_MALLOC_PRIOR_INDEX
	heap_idx = CONFIG_RAM_MALLOC_PRIOR_INDEX;
#endif

	ret = heap_zalloc(size, heap_idx, HEAP_END_IDX, caller_retaddr);
	if (ret != NULL) {
		return ret;
	}

#if (defined(CONFIG_RAM_MALLOC_PRIOR_INDEX) && CONFIG_RAM_MALLOC_PRIOR_INDEX > 0)
	/* Try to mm_calloc to other heaps */
	ret = heap_zalloc(size, HEAP_START_IDX, CONFIG_RAM_MALLOC_PRIOR_INDEX - 1, caller_retaddr);
#endif
#endif /* CONFIG_APP_BINARY_SEPARATION */

	return ret;
#endif /* CONFIG_ARCH_ADDRENV */
}

