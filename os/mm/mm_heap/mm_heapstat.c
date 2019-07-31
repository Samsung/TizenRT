/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

/// @file mm_heapstat.c

/// @brief Retrieve the stat of heap for realtime support such as the number of misses of memory allocation within a constant time.

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <tinyara/mm/mm.h>
#include <stdio.h>

#ifdef CONFIG_EXAMPLES_HEAP_STAT_REALTIME
int mm_heapstat_get(char *addr, size_t *miss, int *num)
{
	struct mm_heap_s *heap;
	int ndx;

	heap = mm_get_heap(addr);
	if (!heap) {
		return -1;
	}

	mm_takesemaphore(heap);
	for (ndx = 0; ndx < MM_REALTIME_SUPPORT_NUMOF_SIZES; ++ndx) {
		miss[ndx] = heap->mm_realtime_miss[ndx];
		num[ndx] = heap->mm_realtime_num[ndx];
	}
	mm_givesemaphore(heap);

	return 0;
}

int mm_heapstat_reset(char *addr)
{
	struct mm_heap_s *heap;
	int ndx;

	heap = mm_get_heap(addr);
	if (!heap) {
		return -1;
	}

	mm_takesemaphore(heap);
	for (ndx = 0; ndx < MM_REALTIME_SUPPORT_NUMOF_SIZES; ++ndx) {
		heap->mm_realtime_miss[ndx] = 0;
	}
	mm_givesemaphore(heap);

	return 0;
}
#endif
