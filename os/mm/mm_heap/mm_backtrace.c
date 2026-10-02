/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * os/mm/mm_heap/mm_backtrace.c
 *
 * Weak fallback for up_backtrace(), used when CONFIG_MM_BACKTRACE > 0 and the
 * EHABI engine (CONFIG_SCHED_BACKTRACE) is not selected.
 *
 * In a kernel build the arch implementation (arm_backtrace_thumb.c or
 * arm_backtrace_fp.c) provides a strong symbol that overrides this stub.  In a
 * user-space (libumm) build there is no arch unwinder, so this stub is linked
 * and reports "no frames" rather than leaving the symbol undefined.
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/mm/mm.h>

/* Runtime-adjustable backtrace skip value.  Initialized to the compile-time
 * default CONFIG_MM_BACKTRACE_SKIP.  Can be changed at runtime via
 * "heapinfo -s <value>".
 */
#if CONFIG_MM_BACKTRACE > 0
int g_mm_backtrace_skip = CONFIG_MM_BACKTRACE_SKIP;
#endif

#if (CONFIG_MM_BACKTRACE > 0) && !defined(CONFIG_SCHED_BACKTRACE)

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_backtrace
 *
 * Description:
 *   Weak no-op implementation.  Returns 0 frames and NULL-terminates the
 *   caller's buffer so MM_ADD_BACKTRACE() records an empty call stack.
 *
 * Returned Value:
 *   Always 0 (no frames captured).
 *
 ****************************************************************************/

int __attribute__((weak)) up_backtrace(FAR struct tcb_s *tcb, FAR void **buffer, int size, int skip)
{
	(void)tcb;
	(void)skip;

	if (buffer && size > 0) {
		buffer[0] = NULL;
	}

	return 0;
}

#endif /* (CONFIG_MM_BACKTRACE > 0) && !defined(CONFIG_SCHED_BACKTRACE) */

