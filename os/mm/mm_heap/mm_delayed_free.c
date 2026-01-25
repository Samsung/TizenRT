/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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

#ifdef CONFIG_MM_DELAYED_FREE_WORKER

#include <tinyara/mm/mm.h>
#include <unistd.h> // For sleep()

#ifdef CONFIG_DEBUG_MM_HEAPINFO
#include <tinyara/sched.h> // For task_self() if needed for heapinfo
#endif

// Assuming g_mmheap is the global pointer to the heap structure.
// This needs to be confirmed or an accessor function used.
extern struct mm_heap_s g_mmheap;

// Prototype for mm_free_delaylist, assuming it's declared in mm_node.h or similar internal header
// If it's static in mm_malloc.c, it needs to be made non-static and declared in a header.
// For now, let's assume it will be made available.
extern void mm_free_delaylist(FAR struct mm_heap_s *heap);

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mm_delayed_free_worker
 *
 * Description:
 *   This is a worker thread that periodically calls mm_free_delaylist
 *   to free memory from the delayed deallocation list.
 *
 * Input Parameters:
 *   argc - Number of arguments
 *   argv - Arguments
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
int mm_delayed_free_worker(int argc, char *argv[])
{
	mllvdbg("mm_delayed_free_worker started\n");

	while (1) {
		// TODO: Iterate over all heaps if multiple heaps are supported
		// For now, using the assumed global g_mmheap
		// Also, ensure mm_free_delaylist is thread-safe or called appropriately.
		if (&g_mmheap != NULL) {
			mmvdbg("Calling mm_free_delaylist for heap %p\n", &g_mmheap);
			mm_free_delaylist(&g_mmheap);
		} else {
			mllvdbg("g_mmheap is NULL, skipping mm_free_delaylist\n");
		}

		sleep(CONFIG_MM_DELAYED_FREE_INTERVAL);
	}

	// This part will not be reached
	mllvdbg("mm_delayed_free_worker exiting\n");
	return 0;
}

#endif /* CONFIG_MM_DELAYED_FREE_WORKER */
