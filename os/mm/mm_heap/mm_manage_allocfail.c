/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#include <string.h>
#include <stdlib.h>
#include <tinyara/mm/mm.h>
#ifdef CONFIG_MM_ASSERT_ON_FAIL
#include <assert.h>
#ifdef CONFIG_SYSTEM_REBOOT_REASON
#include <tinyara/reboot_reason.h>
#endif
#endif

#ifdef CONFIG_MM_ASSERT_ON_FAIL
#define mfdbg lldbg // When CONFIG_MM_ASSERT_ON_FAIL is enabled, we cannot use the buffer way like mdbg because of board assert.
#else
#define mfdbg mdbg
#endif

#define KERNEL_STR "kernel"
#define USER_STR   "user"
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/************************************************************************
 * Name: mm_manage_alloc_fail
 *
 * Description:
 *   Manage the allocation failure case.
 *   If needed, write the reboot reason and show the heap status through heapinfo.
 *   And if CONFIG_MM_ASSERT_ON_FAIL is enabled, make assert.
 *
 * Return Value: None
 *
 ************************************************************************/

void mm_manage_alloc_fail(struct mm_heap_s *heap, int heapidx, size_t size, int heap_type)
{
	mfdbg("Allocation failed from %s heap.\n", (heap_type == KERNEL_HEAP) ? KERNEL_STR : USER_STR);
	mfdbg(" - requested size %u\n", size);

#ifdef CONFIG_MM_ASSERT_ON_FAIL
	struct mallinfo info;
	memset(&info, 0, sizeof(struct mallinfo));
	for (int idx = 0; idx < heapidx; idx++) {
		mm_mallinfo(&heap[idx], &info);
	}
	mfdbg(" - largest free size : %d\n", info.mxordblk);
	mfdbg(" - total free size   : %d\n", info.fordblks);

#ifdef CONFIG_SYSTEM_REBOOT_REASON
	WRITE_REBOOT_REASON(REBOOT_SYSTEM_MEMORYALLOCFAIL);
#endif
#endif /* CONFIG_MM_ASSERT_ON_FAIL */

#ifdef CONFIG_DEBUG_MM_HEAPINFO
	for (int idx = 0; idx < heapidx; idx++) {
		heapinfo_parse_heap(&heap[idx], HEAPINFO_DETAIL_ALL, HEAPINFO_PID_ALL);
	}
#endif

#ifdef CONFIG_MM_ASSERT_ON_FAIL
	PANIC();
#endif
}
