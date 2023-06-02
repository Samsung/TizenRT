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
#endif
#if defined(CONFIG_DEBUG_MM_HEAPINFO) && defined(CONFIG_WATCHDOG)
#include <tinyara/watchdog.h>
#include <fcntl.h>
#endif
#if defined(CONFIG_DEBUG_MM_HEAPINFO) && defined(CONFIG_WATCHDOG) || defined(CONFIG_APP_BINARY_SEPARATION) && !defined(__KERNEL__)
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

#define KERNEL_STR "kernel"
#define USER_STR   "user"
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/


/************************************************************************
 * Name: mm_heapinfo_stop_watchdog
 *
 * Description:
 *   Stop the watchdog.
 *   Forced stop or pause of the watchdog breaks the time guaranteed by the watchdog.
 *   Therefore, if only watchdog is running and CONFIG_MM_ASSERT_ON_FAIL is enabled,
 *   try to stop the watchdog.
 *
 * Return Value:
 *   OK    - stopped watichdog or watchdog is not running.
 *   ERROR - fail to stop watchdog
 *
 ************************************************************************/
#if defined(CONFIG_DEBUG_MM_HEAPINFO) && defined(CONFIG_WATCHDOG)
static inline int mm_heapinfo_stop_watchdog(void)
{
	struct watchdog_status_s wd_status;
	int fd = open(CONFIG_WATCHDOG_DEVPATH, O_RDWR);
	if (fd < 0) {
		mfdbg("Fail to open %s, errno %d\n", CONFIG_WATCHDOG_DEVPATH, get_errno());
		return ERROR;
	}

	if (ioctl(fd, WDIOC_GETSTATUS, (unsigned long)&wd_status) != OK) {
		mfdbg("Fail to get watchdog state, errno %d\n", get_errno());
		return ERROR;
	}

	if (!(wd_status.flags & WDFLAGS_ACTIVE)) {
		/* watchdog is not running */
		return OK;
	}

#ifdef CONFIG_MM_ASSERT_ON_FAIL
	/* watchdog can be stopped only when CONFIG_MM_ASSERT_ON_FAIL is enabled */
	if (ioctl(fd, WDIOC_STOP, 0) == OK) {
		return OK;
	}
	mfdbg("Fail to stop watchdog, errno %d\n", get_errno());
#endif

	return ERROR;
}
#else
#define mm_heapinfo_stop_watchdog() OK
#endif

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
#ifdef CONFIG_DEBUG_MM_HEAPINFO
void mm_ioctl_alloc_fail(size_t size, uint32_t caller)
#else
void mm_ioctl_alloc_fail(size_t size)
#endif
{
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	struct mm_alloc_fail_s arg = {size, caller};
#else
	struct mm_alloc_fail_s arg = {size, 0};
#endif
	int mmfd = open(MMINFO_DRVPATH, O_RDWR);
	if (mmfd < 0) {
		mdbg("Fail to open %s, errno %d\n", MMINFO_DRVPATH, get_errno());
	} else {
		int res = ioctl(mmfd, MMINFOIOC_MNG_ALLOCFAIL, &arg);
		if (res == ERROR) {
			mdbg("Fail to call mm_manage_allocfail, errno %d\n", get_errno());
		}
		close(mmfd);
	}
}

#else

#ifdef CONFIG_DEBUG_MM_HEAPINFO
void mm_manage_alloc_fail(struct mm_heap_s *heap, int startidx, int endidx, size_t size, int heap_type, uint32_t caller)
#else
void mm_manage_alloc_fail(struct mm_heap_s *heap, int startidx, int endidx, size_t size, int heap_type)
#endif
{
	irqstate_t flags = enter_critical_section();

	extern bool abort_mode;
#ifdef CONFIG_MM_ASSERT_ON_FAIL
	abort_mode = true;
#endif

	mfdbg("Allocation failed from %s heap.\n", (heap_type == KERNEL_HEAP) ? KERNEL_STR : USER_STR);
	mfdbg(" - requested size %u\n", size);
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	mfdbg(" - caller address = 0x%08x\n", caller);
#endif

	struct mallinfo info;
	memset(&info, 0, sizeof(struct mallinfo));
	for (int idx = startidx; idx <= endidx; idx++) {
		mm_mallinfo(&heap[idx], &info);
	}
	mfdbg(" - largest free size : %d\n", info.mxordblk);
	mfdbg(" - total free size   : %d\n", info.fordblks);

#ifdef CONFIG_MM_ASSERT_ON_FAIL
#ifdef CONFIG_SYSTEM_REBOOT_REASON
	WRITE_REBOOT_REASON(REBOOT_SYSTEM_MEMORYALLOCFAIL);
#endif

#endif /* CONFIG_MM_ASSERT_ON_FAIL */

#ifdef CONFIG_DEBUG_MM_HEAPINFO
	/* heapinfo_parse_heap() operations can print logs for more than 3 minutes,
	 * which can be rebooted by watchdog. So try to stop the watchdog. */

	int ret = mm_heapinfo_stop_watchdog();

#ifndef CONFIG_MM_ASSERT_ON_FAIL
	/* If CONFIG_MM_ASSERT_ON_FAIL is enabled, will be rebooted anyway, So even if
	 * fail to stop watchdog, print heapinfo_parse_heap log. */

	if (ret == OK)
#endif
	{
		for (int idx = startidx; idx <= endidx; idx++) {
			heapinfo_parse_heap(&heap[idx], HEAPINFO_DETAIL_ALL, HEAPINFO_PID_ALL);
		}
	}

#endif /* CONFIG_DEBUG_MM_HEAPINFO */

#ifdef CONFIG_MM_ASSERT_ON_FAIL
	PANIC();
#endif

	leave_critical_section(flags);
}
#endif
