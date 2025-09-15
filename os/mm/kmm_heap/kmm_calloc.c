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
 * mm/kmm_heap/kmm_calloc.c
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
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
#include <debug.h>
#include <tinyara/mm/mm.h>

#ifdef CONFIG_MM_KERNEL_HEAP

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void *kheap_calloc(size_t n, size_t elem_size, mmaddress_t retaddr)
{
	int heap_idx;
	void *ret;
	struct mm_heap_s *kheap = kmm_get_baseheap();

	for (heap_idx = HEAP_START_IDX; heap_idx <= HEAP_END_IDX; heap_idx++) {
		ret = mm_calloc(&kheap[heap_idx], n, elem_size, retaddr);
		if (ret != NULL) {
			return ret;
		}
	}

	mm_manage_alloc_fail(kheap, HEAP_START_IDX, HEAP_END_IDX, n * elem_size, 0, KERNEL_HEAP, retaddr);
	return NULL;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/************************************************************************
 * Name: kmm_calloc_at
 *
 * Description:
 *   calloc to the specific kernel heap.
 *   kmm_calloc_at tries to allocate memory for a specific heap
 *  which passed by api argument.
 *   If there is no enough space to allocate, it will return NULL.
 *
 * Return Value:
 *   The address of the allocated memory (NULL on failure to allocate)
 *
 ************************************************************************/
#if CONFIG_KMM_NHEAPS > 1
void *kmm_calloc_at(int heap_index, size_t n, size_t elem_size)
{
	void *ret;
	struct mm_heap_s *kheap;

	mmaddress_t caller_retaddr = NULL;	/* for generalising the call to mm_calloc api */
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	caller_retaddr = GET_RETURN_ADDRESS();
#endif
	if (heap_index > HEAP_END_IDX || heap_index < HEAP_START_IDX) {
		mdbg("kmm_calloc_at failed. Wrong heap index (%d) of (%d)\n", heap_index, HEAP_END_IDX);
		return NULL;
	}

	if (n == 0 || elem_size == 0) {
		return NULL;
	}

	kheap = kmm_get_baseheap();
	ret = mm_calloc(&kheap[heap_index], n, elem_size, caller_retaddr);
	if (ret == NULL) {
		mm_manage_alloc_fail(&kheap[heap_index], heap_index, heap_index, n * elem_size, 0, KERNEL_HEAP, caller_retaddr);
	}
	return ret;
}
#endif

/****************************************************************************
 * Name: kmm_calloc
 *
 * Description:
 *   kmm_calloc is a thin wrapper for mm_calloc()
 *
 ****************************************************************************/

FAR void *kmm_calloc(size_t n, size_t elem_size)
{
	if (n == 0 || elem_size == 0) {
		return NULL;
	}

	mmaddress_t caller_retaddr = NULL;	/* for generalising the call to kheap_calloc api */
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	caller_retaddr = GET_RETURN_ADDRESS();
#endif

	return kheap_calloc(n, elem_size, caller_retaddr);
}

#endif							/* CONFIG_MM_KERNEL_HEAP */
