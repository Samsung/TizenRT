/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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

#include <sys/types.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>
#include <sched.h>
#include <assert.h>
#include <errno.h>

#include <tinyara/arch.h>
#include <tinyara/wdog.h>
#include <tinyara/mm/mm.h>

#include "sched/sched.h"
#include "wdog/wdog.h"

static inline void wd_dump(struct wdog_s *wdog)
{
	int index = 0;
	lldbg("Wdog address = 0x%08x\n", wdog);
	lldbg("lag: %d\n", wdog->lag);
	lldbg("flags: %u\n", wdog->flags);
	lldbg("pid: %d\n", wdog->pid);
	lldbg("func: %p\n", wdog->func);
	lldbg("argc: %u\n", wdog->argc);
	for (; index < CONFIG_MAX_WDOGPARMS; index++) {
		lldbg("parm[%d]: %u\n", index, wdog->parm[index]);
	}
}

void wd_corruption_dbg(struct wdog_s *wdog)
{
	uint32_t wd_node;
	lldbg("WDOG INFO\n");
	wd_dump(wdog);

	if (mm_get_heap(wdog)) {

#ifdef CONFIG_DEBUG_MM_HEAPINFO
		// Wdog struct is in heap
		lldbg("WDOG is in HEAP. Print neighbour heap nodes\n");
		wd_node = (uint32_t)wdog - SIZEOF_MM_ALLOCNODE;
		lldbg("PREV WDOG INFO\n");
		mm_dump_node((struct mm_allocnode_s *)(wd_node - (((struct mm_allocnode_s *)wd_node)->preceding & ~MM_ALLOC_BIT)), "PREV HEAP NODE");
		lldbg("NEXT WDOG INFO\n");
		mm_dump_node((struct mm_allocnode_s *)(wd_node + ((struct mm_allocnode_s *)wd_node)->size), "NEXT HEAP NODE");
#endif

	} else if (wd_is_prealloc(wdog)) {
		// Wdog struct is from prealloc list

		lldbg("WDOG is in pre alloc list. Print neighbor wdog nodes\n");

		if (wd_is_prealloc(wdog - 1)) {
			lldbg("PREV WDOG INFO\n");
			wd_dump(wdog - 1);
		} else {
			lldbg("WDOG is first in pre-alloc list\n");
		}

		if (wd_is_prealloc(wdog + 1)) {
			lldbg("NEXT WDOG INFO\n");
			wd_dump(wdog + 1);
		} else {
			lldbg("WDOG is last in pre-alloc list\n");
		}

	} else {
		lldbg("WDOG Address is invalid\n");
	}
}
