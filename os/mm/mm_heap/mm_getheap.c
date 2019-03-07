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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <debug.h>
#include <tinyara/mm/mm.h>
#ifdef CONFIG_MM_KERNEL_HEAP
#include <tinyara/sched.h>
#endif

#if defined(CONFIG_BUILD_PROTECTED) && defined(__KERNEL__)
#include <tinyara/userspace.h>
#define USR_HEAP	(USERSPACE->us_heap)
#elif defined(CONFIG_BUILD_PROTECTED)
#include <tinyara/userspace.h>
extern struct userspace_s g_userspace;
#define USR_HEAP	(g_userspace.us_heap)
#else
extern struct mm_heap_s g_mmheap[CONFIG_MM_NHEAPS];
#define USR_HEAP	g_mmheap
#endif
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: mm_get_heap
 *
 * Description:
 *   returns a heap which type is matched with ttype
 *
 ****************************************************************************/
struct mm_heap_s *mm_get_heap(void *address)
{
#ifdef CONFIG_MM_KERNEL_HEAP
	if (address >= (FAR void *)g_kmmheap.mm_heapstart[0] && address < (FAR void *)g_kmmheap.mm_heapend[0]) {
		return &g_kmmheap;
	}
#endif
	int heap_idx;
	heap_idx = mm_get_heapindex(address);
	if (heap_idx == INVALID_HEAP_IDX) {
		mdbg("address is not in heap region.\n");
		return NULL;
	}

	return &USR_HEAP[heap_idx];
}


/****************************************************************************
 * Name: mm_get_heap_with_index
 ****************************************************************************/
struct mm_heap_s *mm_get_heap_with_index(int index)
{
	if (index >= CONFIG_MM_NHEAPS) {
		mdbg("heap index is out of range.\n");
		return NULL;
	}

	return &USR_HEAP[index];
}

/****************************************************************************
 * Name: mm_get_heapindex
 ****************************************************************************/
int mm_get_heapindex(void *mem)
{
	int heap_idx;

	if (mem == NULL) {
		return 0;
	}
	for (heap_idx = 0; heap_idx < CONFIG_MM_NHEAPS; heap_idx++) {
		if (mem < (void *)(USR_HEAP[heap_idx].mm_heapstart + USR_HEAP[heap_idx].mm_heapsize) && mem >= (void *)USR_HEAP[heap_idx].mm_heapstart) {
			return heap_idx;
		}
	}
	return INVALID_HEAP_IDX;
}
