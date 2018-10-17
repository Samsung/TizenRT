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

/// @file kernel_tc_main.c

/// @brief Main Function for Kernel TestCase Example

#include <tinyara/config.h>
#include <stdio.h>
#include "tc_common.h"
#include "tc_internal.h"

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int tc_kernel_main(int argc, char *argv[])
#endif
{
	if (tc_handler(TC_START, "Kernel TC") == ERROR) {
		return ERROR;
	}

#ifdef CONFIG_TC_KERNEL_TASH_HEAPINFO
	tash_heapinfo_main();
#endif

#ifdef CONFIG_TC_KERNEL_TASH_STACKMONITOR
	tash_stackmonitor_main();
#endif

#ifdef CONFIG_TC_KERNEL_CLOCK
	clock_main();
#endif

#ifdef CONFIG_TC_KERNEL_ENVIRON
	environ_main();
#endif

#ifdef CONFIG_TC_KERNEL_ERRNO
	errno_main();
#endif

#ifdef CONFIG_TC_KERNEL_GROUP
#if (!defined CONFIG_SCHED_HAVE_PARENT) || (!defined CONFIG_SCHED_CHILD_STATUS)
#error CONFIG_SCHED_HAVE_PARENT and CONFIG_SCHED_CHILD_STATUS are needed for testing GROUP TC
#endif
	group_main();
#endif

#ifdef CONFIG_TC_KERNEL_LIBC_FIXEDMATH
	libc_fixedmath_main();
#endif

#ifdef CONFIG_TC_KERNEL_LIBC_INTTYPES
	libc_inttypes_main();
#endif

#ifdef CONFIG_TC_KERNEL_LIBC_LIBGEN
	libc_libgen_main();
#endif

#ifdef CONFIG_TC_KERNEL_LIBC_MATH
	libc_math_main();
#endif

#ifdef CONFIG_TC_KERNEL_LIBC_MISC
#if (!defined CONFIG_DEBUG) || (!defined CONFIG_DEBUG_ERROR) || (!defined CONFIG_DEBUG_VERBOSE)
#error CONFIG_DEBUG, CONFIG_DEBUG_ERROR and CONFIG_DEBUG_VERBOSE are needed for testing LIBC_MISC TC
#endif
	libc_misc_main();
#endif

#ifdef CONFIG_TC_KERNEL_LIBC_MQUEUE
	libc_mqueue_main();
#endif

#ifdef CONFIG_TC_KERNEL_LIBC_PTHREAD
	libc_pthread_main();
#endif

#ifdef CONFIG_TC_KERNEL_LIBC_QUEUE
	libc_queue_main();
#endif

#ifdef CONFIG_TC_KERNEL_LIBC_SCHED
	libc_sched_main();
#endif

#ifdef CONFIG_TC_KERNEL_LIBC_SEMAPHORE
	libc_semaphore_main();
#endif

#ifdef CONFIG_TC_KERNEL_LIBC_SIGNAL
	libc_signal_main();
#endif

#ifdef CONFIG_TC_KERNEL_LIBC_STDIO
	libc_stdio_main();
#endif

#ifdef CONFIG_TC_KERNEL_LIBC_STDLIB
#if (!defined CONFIG_FS_WRITABLE)
#error CONFIG_FS_WRITABLE is needed for testing LIBC_STDLIB TC
#endif
	libc_stdlib_main();
#endif

#ifdef CONFIG_TC_KERNEL_LIBC_STRING
	libc_string_main();
#endif

#ifdef CONFIG_TC_KERNEL_LIBC_SYSLOG
	libc_syslog_main();
#endif

#ifdef CONFIG_TC_KERNEL_LIBC_TIMER
	libc_timer_main();
#endif

#ifdef CONFIG_TC_KERNEL_LIBC_UNISTD
	libc_unistd_main();
#endif

#ifdef CONFIG_TC_KERNEL_MQUEUE
	mqueue_main();
#endif

#ifdef CONFIG_TC_KERNEL_PTHREAD
	pthread_main();
#endif

#ifdef CONFIG_TC_KERNEL_SCHED
	sched_main();
#endif

#ifdef CONFIG_TC_KERNEL_SEMAPHORE
	semaphore_main();
#endif

#ifdef CONFIG_TC_KERNEL_SIGNAL
	signal_main();
#endif

#ifdef CONFIG_TC_KERNEL_TASK
	task_main();
#endif

#ifdef CONFIG_TC_KERNEL_TERMIOS
	termios_main();
#endif

#ifdef CONFIG_TC_KERNEL_TIMER
	timer_main();
#endif

#ifdef CONFIG_TC_KERNEL_ROUNDROBIN
	roundrobin_main();
#endif

#ifdef CONFIG_TC_KERNEL_UMM_HEAP
	umm_heap_main();
#endif

#ifdef CONFIG_TC_KERNEL_WORK_QUEUE
	wqueue_main();
#endif

#ifdef CONFIG_TC_KERNEL_MEMORY_SAFETY
	memory_safety_main();
#endif

#ifdef CONFIG_ITC_KERNEL_ENVIRON
	itc_environ_main();
#endif

#ifdef CONFIG_ITC_KERNEL_LIBC_PTHREAD
	itc_libc_pthread_main();
#endif

#ifdef CONFIG_ITC_KERNEL_LIBC_SEMAPHORE
	itc_libc_semaphore_main();
#endif

#ifdef CONFIG_ITC_KERNEL_SEMAPHORE
	itc_semaphore_main();
#endif

#ifdef CONFIG_ITC_KERNEL_SCHED
	itc_sched_main();
#endif

#ifdef CONFIG_ITC_KERNEL_TIMER
	itc_timer_main();
#endif

#ifdef CONFIG_ITC_KERNEL_PTHREAD
	itc_pthread_main();
#endif

	(void)tc_handler(TC_END, "Kernel TC");

	return 0;
}
