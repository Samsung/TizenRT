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
 * mm/kmm_heap/kmm_malloc.c
 *
 *   Copyright (C) 2014  Gregory Nutt. All rights reserved.
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

#include <tinyara/mm/mm.h>

#ifdef CONFIG_MM_KERNEL_HEAP

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

static void *kheap_malloc(size_t size, size_t retaddr)
{
	int heap_idx;
	void *ret;
	struct mm_heap_s *kheap = kmm_get_heap();

	for (heap_idx = 0; heap_idx < CONFIG_KMM_NHEAPS; heap_idx++) {
#ifdef CONFIG_DEBUG_MM_HEAPINFO
		ret = mm_malloc(&kheap[heap_idx], size, retaddr);
#else
		ret = mm_malloc(&kheap[heap_idx], size);
#endif
		if (ret != NULL) {
			return ret;
		}
	}

	return NULL;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/************************************************************************
 * Name: kmm_malloc_at
 *
 * Description:
 *   Allocate memory to the specific kernel heap.
 *   kmm_malloc_at tries to allocate memory for a specific kernel heap
 *  which passed by api argument.
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
void *kmm_malloc_at(int heap_index, size_t size)
{
	struct mm_heap_s *kheap;
	if (heap_index >= CONFIG_KMM_NHEAPS || heap_index < 0) {
		mdbg("kmm_malloc_at failed. Wrong heap index (%d) of (%d)\n", heap_index, CONFIG_KMM_NHEAPS);
		return NULL;
	}

	kheap = kmm_get_heap();
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	ARCH_GET_RET_ADDRESS
	return mm_malloc(&kheap[heap_index], size, retaddr);
#else
	return mm_malloc(&kheap[heap_index], size);
#endif
}
#endif

/************************************************************************
 * Name: kmm_malloc
 *
 * Description:
 *   Allocate memory from the kernel heap.
 *
 * Parameters:
 *   size - Size (in bytes) of the memory region to be allocated.
 *
 * Return Value:
 *   The address of the allocated memory (NULL on failure to allocate)
 *
 ************************************************************************/

FAR void *kmm_malloc(size_t size)
{
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	ARCH_GET_RET_ADDRESS
#else
	size_t retaddr = 0;
#endif
	return kheap_malloc(size, retaddr);
}
#endif							/* CONFIG_MM_KERNEL_HEAP */
