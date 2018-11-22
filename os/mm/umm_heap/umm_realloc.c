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
 * mm/umm_heap/umm_realloc.c
 *
 *   Copyright (C) 2007, 2009, 2013-2014 Gregory Nutt. All rights reserved.
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
#include <debug.h>
#include <tinyara/mm/mm.h>

#if !defined(CONFIG_BUILD_PROTECTED) || !defined(__KERNEL__)

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
 * Name: realloc_at
 *
 * Description:
 *   realloc to the specific heap.
 *
 * Return Value:
 *   The address of the allocated memory (NULL on failure to allocate)
 *
 ************************************************************************/

#if CONFIG_MM_NHEAPS > 1
void *realloc_at(int heap_index, void *oldmem, size_t size)
{
	if (heap_index >= CONFIG_MM_NHEAPS || heap_index < 0) {
		mdbg("realloc_at failed. Wrong heap index (%d) of (%d)\n", heap_index, CONFIG_MM_NHEAPS);
		return NULL;
	}
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	ARCH_GET_RET_ADDRESS
	return mm_realloc(&g_mmheap[heap_index], oldmem, size, retaddr);
#else
	return mm_realloc(&g_mmheap[heap_index], oldmem, size);
#endif
}
#endif
/****************************************************************************
 * Name: realloc
 *
 * Description:
 *   Re-allocate memory in the user heap.
 *
 * Parameters:
 *   oldmem  - The old memory allocated
 *   newsize - Size (in bytes) of the new memory region to be re-allocated.
 *
 * Return Value:
 *   The address of the re-allocated memory (NULL on failure to re-allocate)
 *
 ****************************************************************************/

FAR void *realloc(FAR void *oldmem, size_t size)
{
	int heap_idx;
	int prev_heap_idx;
	void *ret;
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	ARCH_GET_RET_ADDRESS
#endif
	heap_idx = mm_get_heapindex(oldmem);
	if (heap_idx < 0) {
		return NULL;
	}
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	ret = mm_realloc(&g_mmheap[heap_idx], oldmem, size, retaddr);
#else
	ret = mm_realloc(&g_mmheap[heap_idx], oldmem, size);
#endif
	if (ret != NULL) {
		return ret;
	}
	/* Try to mm_malloc to another heap */
	mdbg("After realloc, memory can be allocated to another heap which is not as same as previous.\n");
	prev_heap_idx = heap_idx;
	for (heap_idx = 0; heap_idx < CONFIG_MM_NHEAPS; heap_idx++) {
#ifdef CONFIG_DEBUG_MM_HEAPINFO
		ret = mm_malloc(&g_mmheap[heap_idx], size, retaddr);
#else
		ret = mm_malloc(&g_mmheap[heap_idx], size);
#endif
		if (ret != NULL) {
			mm_free(&g_mmheap[prev_heap_idx], oldmem);
			return ret;
		}
	}
	return NULL;
}
#endif							/* !CONFIG_BUILD_PROTECTED || !__KERNEL__ */
