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
/****************************************************************************
 * os/binfmt/binfmt_execmodule.c
 *
 *   Copyright (C) 2009, 2013-2014, 2017 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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

#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <sched.h>
#include <debug.h>
#include <errno.h>

#include <tinyara/arch.h>
#include <tinyara/kmalloc.h>
#include <tinyara/mm/shm.h>
#include <tinyara/binfmt/binfmt.h>
#include <tinyara/mm/mm.h>
#if defined(CONFIG_APP_BINARY_SEPARATION) && defined(CONFIG_ARCH_USE_MMU)
#include <tinyara/mmu.h>
#endif

#ifdef CONFIG_BINARY_MANAGER
#include <string.h>
#include "binary_manager/binary_manager.h"
#endif

#include "sched/sched.h"
#include "binfmt.h"

#ifdef CONFIG_BINFMT_ENABLE

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* If C++ constructors are used, then CONFIG_SCHED_STARTHOOK must also be
 * selected be the start hook is used to schedule execution of the
 * constructors.
 */

#if defined(CONFIG_BINFMT_CONSTRUCTORS) && !defined(CONFIG_SCHED_STARTHOOK)
#errror "CONFIG_SCHED_STARTHOOK must be defined to use constructors"
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: exec_ctors
 *
 * Description:
 *   Execute C++ static constructors.  This function is registered as a
 *   start hook and runs on the thread of the newly created task before
 *   the new task's main function is called.
 *
 * Input Parameters:
 *   arg - Argument is instance of load state info structure cast to void *.
 *
 * Returned Value:
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

#ifdef CONFIG_BINFMT_CONSTRUCTORS
static void exec_ctors(FAR void *arg)
{
	FAR const struct binary_s *binp = (FAR const struct binary_s *)arg;
	binfmt_ctor_t *ctor;
	int i;

#ifdef CONFIG_SUPPORT_COMMON_BINARY
	/* If common binary is enabled, we need to run its constructors first */
	if (g_lib_binp->run_library_ctors) {
		ctor = g_lib_binp->ctors;
		for (i = 0; i < g_lib_binp->nctors; i++) {
			binfo("Calling ctor %d at %p\n", i, (FAR void *)ctor);

			(*ctor)();
			ctor++;
		}
		/* unset the flag so that common binary constructors are executed only once */
		g_lib_binp->run_library_ctors = false;
	}
#endif

#ifdef CONFIG_LIBCXX_EXCEPTION
	if (!binp->islibrary) {
		g_lib_binp->register_exidx(binp->exidx_start, binp->exidx_end, binp->sections[BIN_TEXT], binp->sections[BIN_TEXT] + binp->sizes[BIN_TEXT], binp->binary_idx);
	}
#endif

	ctor = binp->ctors;
	/* Execute each constructor */

	for (i = 0; i < binp->nctors; i++) {
		binfo("Calling ctor %d at %p\n", i, (FAR void *)ctor);

		(*ctor)();
		ctor++;
	}
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: exec_module
 *
 * Description:
 *   Execute a module that has been loaded into memory by load_module().
 *
 * Returned Value:
 *   This is a internal function so it follows the convention that
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/
int exec_module(FAR struct binary_s *binp)
{
	FAR struct task_tcb_s *newtcb;
	FAR struct tcb_s *rtcb;
	FAR uint32_t *stack;
	pid_t pid;
	int ret;
	int binary_idx;

	/* Sanity checking */

#ifdef CONFIG_DEBUG
	if (!binp || !binp->entrypt || binp->stacksize <= 0) {
		return -EINVAL;
	}
#endif

	binfo("Executing %s\n", binp->filename);

	binary_idx = binp->binary_idx;
	binp->uheap = (struct mm_heap_s *)binp->sections[BIN_HEAP];
	ret = mm_initialize(binp->uheap, (void *)binp->sections[BIN_HEAP] + sizeof(struct mm_heap_s), binp->sizes[BIN_HEAP]);
	if (ret != OK) {
		berr("ERROR: mm_initialize() failed, %d.\n", ret);
		return ret;
	}
	mm_add_app_heap_list(binp->uheap, binp->bin_name);

	binfo("------------------------%s Binary Heap Information------------------------\n", binp->bin_name);
	binfo("Start addr = 0x%x, size = %u \n", (void *)binp->sections[BIN_HEAP] + sizeof(struct mm_heap_s), binp->sizes[BIN_HEAP]);

	/* The first 4 bytes of the data section of the application must contain a
	pointer to the application's mm_heap object. Here we will store the mm_heap
	pointer to the start of the text section */
	*(uint32_t *)(binp->sections[BIN_DATA]) = (uint32_t)binp->uheap;
	rtcb = (struct tcb_s *)sched_self();
	rtcb->uheap = (uint32_t)binp->uheap;

	/* Allocate a TCB for the new task. */

	newtcb = (FAR struct task_tcb_s *)kmm_zalloc(sizeof(struct task_tcb_s));
	if (!newtcb) {
		ret = -ENOMEM;
		goto errout_with_appheap;
	}

	ret = up_create_stack((FAR struct tcb_s *)newtcb, binp->stacksize, TCB_FLAG_TTYPE_TASK);
	if (ret < 0) {
		berr("ERROR: up_create_stack() failed.\n");
		goto errout_with_appheap;
	}
	stack = newtcb->cmn.stack_alloc_ptr;

	/* Initialize the task */

	ret = task_init((FAR struct tcb_s *)newtcb, binp->filename, binp->priority, NULL, binp->stacksize, binp->entrypt, binp->argv);
	if (ret < 0) {
		ret = -get_errno();
		berr("task_init() failed: %d\n", ret);
		goto errout_with_stack;
	}

#if defined(CONFIG_DEBUG_MM_HEAPINFO)
	/* Re-initialize the binary heap alloc list information.
	 * Loading thread uses the binary's heap for loading,
	 * so it saves the alloc data in binary's heap.
	 * But loading thread is not in its binary, exclude those data
	 * for calculating the heap usage per threads.
	 */
	int hashpid = PIDHASH(getpid());

	binp->uheap->alloc_list[hashpid].pid = HEAPINFO_INIT_INFO;
	binp->uheap->alloc_list[hashpid].curr_alloc_size = 0;
	binp->uheap->alloc_list[hashpid].num_alloc_free = 0;

	/* Exclude a stack node from heap usages of current thread.
	 * This will be shown separately as stack usages.
	 */
	heapinfo_exclude_stacksize(newtcb->cmn.stack_alloc_ptr);
	/* Update the pid information to set a stack node */
	heapinfo_set_stack_node(stack, newtcb->cmn.pid);
#endif

	/* We can free the argument buffer now.
	 * REVISIT:  It is good to free up memory as soon as possible, but
	 * unfortunately here 'binp' is 'const'.  So to do this properly, we will
	 * have to make some more extensive changes.
	 */

	binfmt_freeargv((FAR struct binary_s *)binp);

#if defined(CONFIG_BUILD_KERNEL) && defined(CONFIG_MM_SHM)
	/* Initialize the shared memory virtual page allocator */

	ret = shm_group_initialize(newtcb->cmn.group);
	if (ret < 0) {
		berr("ERROR: shm_group_initialize() failed: %d\n", ret);
		goto errout_with_tcbinit;
	}
#endif

#ifdef CONFIG_PIC
	/* Add the D-Space address as the PIC base address.  By convention, this
	 * must be the first allocated address space.
	 */

	newtcb->cmn.dspace = binp->sections[BIN_TEXT];

	/* Re-initialize the task's initial state to account for the new PIC base */

	up_initial_state(&newtcb->cmn);
#endif

#ifdef CONFIG_BINFMT_CONSTRUCTORS
	/* Setup a start hook that will execute all of the C++ static constructors
	 * on the newly created thread. Common binary static constructions are also
	 * executed during the first app's start hook. The struct binary_s must persist
	 * at least until the new task has been started.
	 */

	task_starthook(newtcb, exec_ctors, (FAR void *)binp);
#endif

	/* Get the assigned pid before we start the task */

	pid = newtcb->cmn.pid;

	/* Parent task rtcb was initialized with uheap and mpu_regs values.
	 * These values are used for stack allocation and task_init and will
	 * be passed on to all the child tasks of the user app. Since the user
	 * app is now created, we will reset these values here
	 */
	rtcb->uheap = 0;
#ifdef CONFIG_ARM_MPU
	memset(rtcb->mpu_regs, 0, sizeof(rtcb->mpu_regs));
#endif
#ifdef CONFIG_ARCH_USE_MMU
	rtcb->pgtbl = mmu_get_os_l1_pgtbl();
#endif

	/* Store the address of the applications userspace object in the newtcb  */
	/* The app's userspace object will be found at an offset of 4 bytes from the start of the binary */
	newtcb->cmn.uspace = binp->sections[BIN_TEXT] + 4;
	newtcb->cmn.uheap = (uint32_t)binp->uheap;

#ifdef CONFIG_BINARY_MANAGER
	newtcb->cmn.app_id = binp->binary_idx;

	/* Set task name as binary name */
	strncpy(newtcb->cmn.name, binp->bin_name, CONFIG_TASK_NAME_SIZE);
	newtcb->cmn.name[CONFIG_TASK_NAME_SIZE] = '\0';

	newtcb->cmn.group->tg_binidx = binary_idx;
	binary_manager_add_binlist(&newtcb->cmn);

	/* Update id and state in binary table */
	BIN_ID(binary_idx) = pid;
	BIN_STATE(binary_idx) = BINARY_LOADED;
	BIN_LOADVER(binary_idx) = binp->bin_ver;
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	BIN_LOADINFO(binary_idx) = binp;
#endif
#endif

#ifndef CONFIG_DISABLE_SIGNALS
	/* Clean the signal mask of loaded task because it inherits signal mask from parent task, binary manager. */
	newtcb->cmn.sigprocmask = NULL_SIGNAL_SET;
#endif

	/* Set up to unload the module when the task exists. */

	newtcb->bininfo = binp;

	/* Then activate the task at the provided priority */

	ret = task_activate((FAR struct tcb_s *)newtcb);
	if (ret < 0) {
		ret = -get_errno();
		berr("task_activate() failed: %d\n", ret);
		goto errout_with_tcbinit;
	}

	binfo("%s loaded @ 0x%08x and running with pid = %d\n", binp->filename, binp->sections[BIN_TEXT], pid);

	return (int)pid;

errout_with_appheap:
	mm_remove_app_heap_list(binp->uheap);
errout_with_tcbinit:
	if (newtcb != NULL) {
#ifdef CONFIG_BINARY_MANAGER
		BIN_ID(binary_idx) = -1;
		BIN_STATE(binary_idx) = BINARY_INACTIVE;
		BIN_LOADVER(binary_idx) = 0;
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
		BIN_LOADINFO(binary_idx) = NULL;
#endif
		binary_manager_remove_binlist(&newtcb->cmn);
#endif
		sched_releasetcb(&newtcb->cmn, TCB_FLAG_TTYPE_TASK);
	}
	return ret;

errout_with_stack:
	kumm_free(stack);
	kmm_free(newtcb);
	return ret;
}

#endif							/* CONFIG_BINFMT_ENABLE */
