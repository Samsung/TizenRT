/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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
#include <sys/types.h>
#include <tinyara/mm/mm.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static size_t mm_get_largest_freesize_from_specific_heap(struct mm_heap_s *heap)
{
	size_t largest_size = 0;
	struct mm_freenode_s *fnode;
	int nodelist_idx = 0;

	/* Free nodes are sorted in a descending order,
	 * so the first node in each nodelist is the largest within its nodelist.
	 */
	for (nodelist_idx = MM_NNODES; nodelist_idx > 0; --nodelist_idx) {
		fnode = heap->mm_nodelist[nodelist_idx].flink;
		if (fnode && largest_size < fnode->size) {
			/* If we meet the node, then it can be the largest free node. */
			largest_size = fnode->size;
			break;
		}
	}
	return largest_size;
}

/****************************************************************************
 * Name: mm_get_largest_freenode_size
 *
 * Description:
 *   Returns the largest free node size in the heap
 ****************************************************************************/

size_t mm_get_largest_freenode_size(void)
{
	struct mm_heap_s *heap;
	size_t largest_size = 0;

#ifdef __KERNEL__
	size_t curheap_freesize;
	int heap_idx;
	heap = g_kmmheap;
	struct mm_heap_s *target_heap;

	/* Kernel can have multi-heap, so traversing all n-heaps to check the largest. */
	for (heap_idx = HEAP_START_IDX; heap_idx <= HEAP_END_IDX; heap_idx++) {
		target_heap = &heap[heap_idx];
		curheap_freesize = mm_get_largest_freesize_from_specific_heap(target_heap);
		if (curheap_freesize > largest_size) {
			largest_size = curheap_freesize;
		}
	}
#else
#if CONFIG_TASK_NAME_SIZE > 0
	char appname[CONFIG_TASK_NAME_SIZE + 1] = { 0 };
#endif
	/* User can only have single heap. */
	heap = BASE_HEAP;
	if (!heap) {
#if CONFIG_TASK_NAME_SIZE > 0
		prctl(PR_GET_NAME, appname);
		mdbg("Failed to retrieve the %s heap region.\n", appname);
#else
		mdbg("Failed to retrieve the app heap region.\n");
#endif
		return 0;
	}
	largest_size = mm_get_largest_freesize_from_specific_heap(heap);
#endif

	return largest_size;
}

/****************************************************************************
 * Name: mm_get_heap_free_size
 *
 * Description:
 *   Returns free size in the heap
 ****************************************************************************/
#ifdef CONFIG_DEBUG_MM_HEAPINFO
size_t mm_get_heap_free_size(void)
{
	struct mm_heap_s *heap;

#ifdef __KERNEL__
	size_t free_size = 0;
	int heap_idx;
	heap = g_kmmheap;
	struct mm_heap_s *target_heap;

	/* Kernel can have multi-heap, so traversing all n-heaps and add all free sizes. */
	for (heap_idx = HEAP_START_IDX; heap_idx <= HEAP_END_IDX; heap_idx++) {
		target_heap = &heap[heap_idx];
		free_size += (target_heap->mm_heapsize - target_heap->total_alloc_size);
	}

	return free_size;
#else
#if CONFIG_TASK_NAME_SIZE > 0
	char appname[CONFIG_TASK_NAME_SIZE + 1] = { 0 };
#endif
	/* User can only have single heap. */
	heap = BASE_HEAP;
	if (!heap) {
#if CONFIG_TASK_NAME_SIZE > 0
		prctl(PR_GET_NAME, appname);
		mdbg("Failed to retrieve the %s heap region.\n", appname);
#else
		mdbg("Failed to retrieve the app heap region.\n");
#endif
		return 0;
	}
	return (heap->mm_heapsize - heap->total_alloc_size);
#endif
}
#endif
