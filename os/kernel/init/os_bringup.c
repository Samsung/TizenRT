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
/****************************************************************************
 * kernel/init/os_bringup.c
 *
 *   Copyright (C) 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * With extensions by:
 *
 *   Author: Uros Platise <uros.platise@isotel.eu>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sched.h>
#include <stdlib.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/init.h>
#include <tinyara/kthread.h>
#include <tinyara/userspace.h>
#include <tinyara/net/net.h>
#ifdef CONFIG_LOGM
#include <tinyara/logm.h>
#endif
#ifdef CONFIG_ENABLE_HEAPINFO
#include <tinyara/heapinfo_drv.h>
#endif
#ifdef CONFIG_TASK_MANAGER
#include <tinyara/task_manager_drv.h>
#endif
#include "wqueue/wqueue.h"
#include "init/init.h"
#ifdef CONFIG_PAGING
#include "paging/paging.h"
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration */

#if defined(CONFIG_INIT_NONE)
/* Kconfig logic will set CONFIG_INIT_NONE if dependencies are not met */

#error No initialization mechanism selected (CONFIG_INIT_NONE)

#else
#if !defined(CONFIG_INIT_ENTRYPOINT)
/* For backward compatibility with older defconfig files when this was
 * the way things were done.
 */

#define CONFIG_INIT_ENTRYPOINT 1
#endif

#if defined(CONFIG_INIT_ENTRYPOINT)
/* Initialize by starting a task at an entry point */

#if !defined(CONFIG_USER_ENTRYPOINT) && !defined(CONFIG_TASH)
/* Entry point name must have been provided */

#error CONFIG_USER_ENTRYPOINT must be defined
#endif
#endif
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: os_pgworker
 *
 * Description:
 *   Start the page fill worker kernel thread that will resolve page faults.
 *   This should always be the first thread started because it may have to
 *   resolve page faults in other threads
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#ifdef CONFIG_PAGING
static inline void os_pgworker(void)
{
	/* Start the page fill worker kernel thread that will resolve page faults.
	 * This should always be the first thread started because it may have to
	 * resolve page faults in other threads
	 */

	svdbg("Starting paging thread\n");

	g_pgworker = kernel_thread("pgfill", CONFIG_PAGING_DEFPRIO, CONFIG_PAGING_STACKSIZE, (main_t)pg_worker, (FAR char *const *)NULL);
	DEBUGASSERT(g_pgworker > 0);
}

#else							/* CONFIG_PAGING */
#define os_pgworker()

#endif							/* CONFIG_PAGING */

/****************************************************************************
 * Name: os_workqueues
 *
 * Description:
 *   Start the worker threads that service the work queues.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#ifdef CONFIG_SCHED_WORKQUEUE
static inline void os_workqueues(void)
{
#ifdef CONFIG_SCHED_HPWORK
	/* Start the high-priority worker thread to support device driver lower
	 * halves.
	 */

	(void)work_hpstart();

#endif							/* CONFIG_SCHED_HPWORK */

#ifdef CONFIG_SCHED_LPWORK
	/* Start the low-priority worker thread for other, non-critical continuation
	 * tasks
	 */

	(void)work_lpstart();

#endif							/* CONFIG_SCHED_LPWORK */
}

#else							/* CONFIG_SCHED_WORKQUEUE */
#define os_workqueues()

#endif							/* CONFIG_SCHED_WORKQUEUE */

/****************************************************************************
 * Name: os_start_application
 *
 * Description:
 *   Execute the board initialization function (if so configured) and start
 *   the application initialization thread.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#if defined(CONFIG_INIT_ENTRYPOINT)
static inline void os_do_appstart(void)
{
	int pid;

#ifdef CONFIG_BOARD_INITIALIZE
	/* Perform any last-minute, board-specific initialization, if so
	 * configured.
	 */

	board_initialize();
#endif

#ifdef CONFIG_SE
	se_initialize();
#endif

#ifdef CONFIG_NET
	/* Initialize the network system & Create network task if required */

	net_initialize();
#endif

	/* Start the application initialization task.  In a flat build, this is
	 * entrypoint is given by the definitions, CONFIG_USER_ENTRYPOINT.  In
	 * the protected build, however, we must get the address of the
	 * entrypoint from the header at the beginning of the user-space blob.
	 */

	svdbg("Starting application init thread\n");


#ifdef CONFIG_SYSTEM_PREAPP_INIT
#ifdef CONFIG_BUILD_PROTECTED
	DEBUGASSERT(USERSPACE->preapp_start != NULL);
	pid = task_create("appinit", SCHED_PRIORITY_DEFAULT, CONFIG_SYSTEM_PREAPP_STACKSIZE, USERSPACE->preapp_start, (FAR char *const *)NULL);
#else
	pid = task_create("appinit", SCHED_PRIORITY_DEFAULT, CONFIG_SYSTEM_PREAPP_STACKSIZE, preapp_start, (FAR char *const *)NULL);
#endif
	if (pid < 0) {
		svdbg("Failed to create application init thread\n");
	}
#endif

#ifdef CONFIG_ENABLE_HEAPINFO
	heapinfo_drv_register();
#endif

#ifdef CONFIG_TASK_MANAGER
#define TASKMGR_STACK_SIZE 2048
#define TASKMGR_PRIORITY 200

	svdbg("Starting task manager\n");
	task_manager_drv_register();

	pid = task_create("task_manager", TASKMGR_PRIORITY, TASKMGR_STACK_SIZE, task_manager, (FAR char *const *)NULL);
	if (pid < 0) {
		svdbg("Failed to create Task Manager\n");
	}
#endif
	svdbg("Starting application main thread\n");
#ifdef CONFIG_BUILD_PROTECTED
	if (USERSPACE->us_entrypoint != NULL) {
		pid = task_create("appmain", SCHED_PRIORITY_DEFAULT, CONFIG_USERMAIN_STACKSIZE, USERSPACE->us_entrypoint, (FAR char *const *)NULL);
	}
#elif defined(CONFIG_USER_ENTRYPOINT)
	pid = task_create("appmain", SCHED_PRIORITY_DEFAULT, CONFIG_USERMAIN_STACKSIZE, (main_t)CONFIG_USER_ENTRYPOINT, (FAR char *const *)NULL);
#endif
	ASSERT(pid > 0);
}

#elif defined(CONFIG_INIT_NONE)
#define os_do_appstart()

#else
#error "Cannot start initialization thread"

#endif

/****************************************************************************
 * Name: os_start_task
 *
 * Description:
 *   This is the framework for a short duration worker thread.  It off-loads
 *   the board initialization and application start-up from the limited
 *   start-up, initialization thread to a more robust kernel thread.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#ifdef CONFIG_BOARD_INITTHREAD
static int os_start_task(int argc, FAR char **argv)
{
	/* Do the board/application initialization and exit */

	os_do_appstart();
	return OK;
}
#endif

/****************************************************************************
 * Name: os_start_application
 *
 * Description:
 *   Execute the board initialization function (if so configured) and start
 *   the application initialization thread.  This will be done either on the
 *   thread of execution of the caller or on a separate thread of execution
 *   if so configured.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static inline void os_start_application(void)
{
#ifdef CONFIG_BOARD_INITTHREAD
	int pid;

	/* Do the board/application initialization on a separate thread of
	 * execution.
	 */

	pid = kernel_thread("AppBringUp", CONFIG_BOARD_INITTHREAD_PRIORITY, CONFIG_BOARD_INITTHREAD_STACKSIZE, (main_t)os_start_task, (FAR char *const *)NULL);

	ASSERT(pid > 0);

#else
	/* Do the board/application initialization on this thread of execution. */

	os_do_appstart();

#endif
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: os_bringup
 *
 * Description:
 *   Start all initial system tasks.  This does the "system bring-up" after
 *   the conclusion of basic OS initialization.  These initial system tasks
 *   may include:
 *
 *   - pg_worker:   The page-fault worker thread (only if CONFIG_PAGING is
 *                  defined.
 *   - work_thread: The work thread.  This general thread can be used to
 *                  perform most any kind of queued work.  Its primary
 *                  function is to serve as the "bottom half" of device
 *                  drivers.
 *
 *   And the main application entry point:
 *   symbols, either:
 *
 *   - CONFIG_USER_ENTRYPOINT: This is the default user application entry
 *                 point
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

int os_bringup(void)
{
	/* Setup up the initial environment for the idle task.  At present, this
	 * may consist of only the initial PATH variable.  The PATH variable is
	 * (probably) not used by the IDLE task.  However, the environment
	 * containing the PATH variable will be inherited by all of the threads
	 * created by the IDLE task.
	 */

#if !defined(CONFIG_DISABLE_ENVIRON) && defined(CONFIG_PATH_INITIAL)
	(void)setenv("PATH", CONFIG_PATH_INITIAL, 1);
#endif

	/* Start the page fill worker kernel thread that will resolve page faults.
	 * This should always be the first thread started because it may have to
	 * resolve page faults in other threads
	 */

	os_pgworker();

	/* Start the worker thread that will serve as the device driver "bottom-
	 * half" and will perform misc garbage clean-up.
	 */

	os_workqueues();

#ifdef CONFIG_LOGM
	logm_start();
#endif

#if !defined(CONFIG_BUILD_KERNEL) && defined(CONFIG_HAVE_CXXINITIALIZE)
	up_cxxinitialize();
#endif

	/* Once the operating system has been initialized, the system must be
	 * started by spawning the user initialization thread of execution.  This
	 * will be the first user-mode thread.
	 */

	os_start_application();


	/* We an save a few bytes by discarding the IDLE thread's environment. */

#if !defined(CONFIG_DISABLE_ENVIRON) && defined(CONFIG_PATH_INITIAL)
	(void)clearenv();
#endif

	return OK;
}
