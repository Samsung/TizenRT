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
 * mm/kmm_heap/kmm_realloc.c
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

#include <tinyara/mm/mm.h>

#ifdef CONFIG_MM_KERNEL_HEAP

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/************************************************************************
 * Name: kmm_realloc_at
 *
 * Description:
 *   realloc to the specific kernel heap.
 *   kmm_realloc_at tries to allocate memory for a specific kernel heap
 *  which passed by api argument.
 *   If there is no enough space to allocate, it will return NULL.
 *
 * Return Value:
 *   The address of the allocated memory (NULL on failure to allocate)
 *
 ************************************************************************/
#if CONFIG_KMM_NHEAPS > 1
void *kmm_realloc_at(int heap_index, void *oldmem, size_t size)
{
	struct mm_heap_s *kheap;
	if (heap_index >= CONFIG_KMM_NHEAPS || heap_index < 0) {
		mdbg("kmm_realloc_at failed. Wrong heap index (%d) of (%d)\n", heap_index, CONFIG_KMM_NHEAPS);
		return NULL;
	}

	kheap = kmm_get_heap();
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	ARCH_GET_RET_ADDRESS
	return mm_realloc(&kheap[heap_index], oldmem, size, retaddr);
#else
	return mm_realloc(&kheap[heap_index], oldmem, size);
#endif
}
#endif

/****************************************************************************
 * Name: kmm_realloc
 *
 * Description:
 *   Re-allocate memory in the kernel heap.
 *
 * Parameters:
 *   oldmem  - The old memory allocated
 *   newsize - Size (in bytes) of the new memory region to be re-allocated.
 *
 * Return Value:
 *   The address of the re-allocated memory (NULL on failure to re-allocate)
 *
 ****************************************************************************/

FAR void *kmm_realloc(FAR void *oldmem, size_t newsize)
{
	void *ret;
	int kheap_idx;
	struct mm_heap_s *kheap_origin = mm_get_heap(oldmem);
	struct mm_heap_s *kheap_new;
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	ARCH_GET_RET_ADDRESS
	ret = mm_realloc(kheap_origin, oldmem, newsize, retaddr);
#else
	ret = mm_realloc(kheap_origin, oldmem, newsize);
#endif
	if (ret != NULL) {
		return ret;
	}

	/* Try to mm_malloc to another heap. */
	kheap_new = kmm_get_heap();
	for (kheap_idx = 0; kheap_idx < CONFIG_KMM_NHEAPS; kheap_idx++) {
#ifdef CONFIG_DEBUG_MM_HEAPINFO
		ARCH_GET_RET_ADDRESS
		ret = mm_malloc(&kheap_new[kheap_idx], newsize, retaddr);
#else
		ret = mm_malloc(&kheap_new[kheap_idx], newsize);
#endif
		if (ret != NULL) {
			kmm_free(oldmem);
			return ret;
		}
	}
	return NULL;
}

#endif							/* CONFIG_MM_KERNEL_HEAP */
