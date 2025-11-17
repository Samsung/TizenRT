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
#if defined(CONFIG_APP_BINARY_SEPARATION) && !defined(__KERNEL__)
#include <stdio.h>
#include <fcntl.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/mminfo.h>
#include <errno.h>
#endif
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

#if defined(CONFIG_APP_BINARY_SEPARATION) && defined(__KERNEL__) && defined(CONFIG_DEBUG_MM_HEAPINFO)
#include "binary_manager/binary_manager_internal.h"
#include "sched/sched.h"
#include <tinyara/binfmt/binfmt.h>
#endif

#include <tinyara/security_level.h>

#define KERNEL_STR "kernel"
#define USER_STR   "user"

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

#if defined(CONFIG_APP_BINARY_SEPARATION) && !defined(__KERNEL__)
void mm_ioctl_alloc_fail(size_t size, size_t align, mmaddress_t caller)
{
	struct mm_alloc_fail_s arg = {size, align, caller};

	int mmfd = open(MMINFO_DRVPATH, O_RDWR);
	if (mmfd < 0) {
		mdbg("Fail to open %s, errno %d\n", MMINFO_DRVPATH, get_errno());
	} else {
		int res = ioctl(mmfd, MMINFOIOC_MNG_ALLOCFAIL, &arg);	/* might need ti handle passing of NULL*/
		if (res == ERROR) {
			mdbg("Fail to call mm_manage_allocfail, errno %d\n", get_errno());
		}
		close(mmfd);
	}
}

void mm_ioctl_garbagecollection(void)
{
	int mmfd = open(MMINFO_DRVPATH, O_RDWR);
	if (mmfd < 0) {
		mdbg("Fail to open %s, errno %d\n", MMINFO_DRVPATH, get_errno());
	} else {
		int res = ioctl(mmfd, MMINFOIOC_GC, NULL);
		if (res == ERROR) {
			mdbg("Fail to call sched_garbagecollection, errno %d\n", get_errno());
		}
		close(mmfd);
	}
}

#else

void mm_manage_alloc_fail_dump(struct mm_heap_s *heap, int startidx, int endidx, size_t size, size_t align, int heap_type, mmaddress_t caller)
{
#ifdef CONFIG_SMP
	/* If SMP is enabled then we need to pause all the other cpu's immediately.
	 * If we don't pause the other CPUs, it might mix up the logs with other
	 * core's printing log.
	 */
	up_cpu_pause_all();
#endif
	extern bool abort_mode;
#ifdef CONFIG_MM_ASSERT_ON_FAIL
	abort_mode = true;
#endif

	mfdbg("Allocation failed from %s heap.\n", (heap_type == KERNEL_HEAP) ? KERNEL_STR : USER_STR);
	mfdbg(" - requested size %u\n", size);
	if (align) {
		mfdbg(" - requested alignment %u\n", align);
	}
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	mfdbg(" - caller address = 0x%08x\n", caller);
#endif

	struct mallinfo info;
	memset(&info, 0, sizeof(struct mallinfo));
	for (int idx = startidx; idx <= endidx; idx++) {
		mm_mallinfo(&heap[idx], &info);
	}

	if (align) {
		mfdbg(" - largest un-aligned free size : %d\n", info.mxordblk);
		memset(&info, 0, sizeof(struct mallinfo));
		for (int idx = startidx; idx <= endidx; idx++) {
			mm_mallinfo_aligned(&heap[idx], &info, align);
		}
		mfdbg(" - largest algined free size : %d\n", info.mxordblk);
	} else {
		mfdbg(" - largest free size : %d\n", info.mxordblk);
	}
	mfdbg(" - total free size   : %d\n", info.fordblks);

#ifdef CONFIG_DEBUG_MM_HEAPINFO
#ifdef CONFIG_APP_BINARY_SEPARATION
	int cur_app_idx = 0;
	if (heap_type != KERNEL_HEAP) {
		cur_app_idx = this_task()->app_id;
		mfdbg("*************************************************************************************\n");
		mfdbg("             Summary of current app (app%d) heap memory usage                        \n", cur_app_idx);
		mfdbg("*************************************************************************************\n\n");
	} else {
		mfdbg("*************************************************************************************\n");
		mfdbg("                          Summary of Kernel heap memory usage                        \n");
		mfdbg("*************************************************************************************\n\n");
	}
#endif

	for (int idx = startidx; idx <= endidx; idx++) {
		heapinfo_parse_heap(&heap[idx], HEAPINFO_DETAIL_ALL, HEAPINFO_PID_ALL);
	}

#ifdef CONFIG_APP_BINARY_SEPARATION
	for (int index = 1; index <= CONFIG_NUM_APPS; index++) {
		if (index != cur_app_idx) {
			mfdbg("*************************************************************************************\n");
			mfdbg("                      Summary of app%d heap memory usage                             \n", index);
			mfdbg("*************************************************************************************\n\n");
			struct mm_heap_s *app_heap = BIN_BINARY_HEAP_PTR(index);
			for (int idx = HEAP_START_IDX; idx <= HEAP_END_IDX; idx++) {
				heapinfo_parse_heap(&app_heap[idx], HEAPINFO_SIMPLE, HEAPINFO_PID_ALL);
			}
		}
	}
	if (heap_type != KERNEL_HEAP) {
		struct mm_heap_s *kheap = kmm_get_baseheap();
		mfdbg("*************************************************************************************\n");
		mfdbg("                          Summary of Kernel heap memory usage                        \n");
		mfdbg("*************************************************************************************\n\n");
		for (int idx = HEAP_START_IDX; idx <= HEAP_END_IDX; idx++) {
			heapinfo_parse_heap(&kheap[idx], HEAPINFO_SIMPLE, HEAPINFO_PID_ALL);
		}
	}
#endif /* CONFIG_APP_BINARY_SEPARATION */
#endif /* CONFIG_DEBUG_MM_HEAPINFO */

#ifdef CONFIG_SMP
	/* If SMP is enabled then we need to resume all the other cpu's
	 * which we paused earlier.
	 */
	up_cpu_resume_all();
#endif
}

void mm_manage_alloc_fail(struct mm_heap_s *heap, int startidx, int endidx, size_t size, size_t align, int heap_type, mmaddress_t caller)
{
	irqstate_t flags = enter_critical_section();

#ifdef CONFIG_MM_ASSERT_ON_FAIL
#ifdef CONFIG_SYSTEM_REBOOT_REASON
	WRITE_REBOOT_REASON(REBOOT_SYSTEM_MEMORYALLOCFAIL);
#endif
#endif /* CONFIG_MM_ASSERT_ON_FAIL */

	/* If secure state, do not print memory usage and address infomation */
	if (!IS_SECURE_STATE()) {
		mm_manage_alloc_fail_dump(heap, startidx, endidx, size, align, heap_type, caller);
	}

#ifdef CONFIG_MM_ASSERT_ON_FAIL
	PANIC();
#endif

	leave_critical_section(flags);
}
#endif
