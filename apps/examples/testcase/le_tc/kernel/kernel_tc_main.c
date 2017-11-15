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
#include <semaphore.h>
#include "tc_internal.h"

extern sem_t tc_sem;
extern int working_tc;

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int kernel_tc_main(int argc, char *argv[])
#endif
{
	sem_wait(&tc_sem);
	working_tc++;

	total_pass = 0;
	total_fail = 0;

	printf("=== TINYARA Kernel TC START! ===\n");

#ifdef CONFIG_TC_KERNEL_TASH_HEAPINFO
	tash_heapinfo_main();
#endif

#ifdef CONFIG_TC_KERNEL_TASH_STACKMONITOR
	tash_stackmonitor_main();
#endif

#ifdef CONFIG_TC_KERNEL_CLOCK
#if (!defined CONFIG_SYSTEM_TIME64)
#error CONFIG_SYSTEM_TIME64 is needed for testing CLOCK TC
#endif
	clock_main();
#endif

#ifdef CONFIG_TC_KERNEL_ENVIRON
	environ_main();
#endif

#ifdef CONFIG_TC_KERNEL_ERRNO
#if (!defined CONFIG_LIBC_STRERROR)
#error CONFIG_LIBC_STRERROR is needed for testing ERRNO TC
#endif
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

#ifdef CONFIG_TC_KERNEL_LIBC_SPAWN
	libc_spawn_main();
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
#if (!defined CONFIG_LIBC_STRERROR)
#error CONFIG_LIBC_STRERROR is needed for testing LIBC_STRING TC
#endif
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
#if (!defined CONFIG_SCHED_HAVE_PARENT)
	/* #error CONFIG_SCHED_HAVE_PARENT is needed for testing SCHED TC */
#endif
	sched_main();
#endif

#ifdef CONFIG_TC_KERNEL_SEMAPHORE
#if (!defined CONFIG_DEBUG) || (!defined CONFIG_SEM_PREALLOCHOLDERS) || (!defined CONFIG_PRIORITY_INHERITANCE)
#error CONFIG_DEBUG, CONFIG_SEM_PHDEBUG, CONFIG_SEM_PREALLOCHOLDERS and CONFIG_PRIORITY_INHERITANCE are needed for testing SEMAPHORE TC
#endif
	semaphore_main();
#endif

#ifdef CONFIG_TC_KERNEL_SIGNAL
	signal_main();
#endif

#ifdef CONFIG_TC_KERNEL_TASK
#if (!defined CONFIG_SCHED_ATEXIT) || (!defined CONFIG_SCHED_ONEXIT) || (!defined CONFIG_TASK_NAME_SIZE)
#error CONFIG_SCHED_ATEXIT, CONFIG_SCHED_ONEXIT and CONFIG_TASK_NAME_SIZE are needed for testing TASK TC
#endif
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

	printf("\n=== TINYARA Kernel TC COMPLETE ===\n");
	printf("\t\tTotal pass : %d\n\t\tTotal fail : %d\n", total_pass, total_fail);

	working_tc--;
	sem_post(&tc_sem);

	return 0;
}
