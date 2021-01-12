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
#include <tinyara/mpu.h>
#include <tinyara/mm/mm.h>

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
	binfmt_ctor_t *ctor = binp->ctors;
	int i;

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
	FAR struct task_tcb_s *tcb;
#ifdef CONFIG_APP_BINARY_SEPARATION
	FAR struct tcb_s *rtcb;
#endif
#if defined(CONFIG_ARCH_ADDRENV) && defined(CONFIG_BUILD_KERNEL)
	save_addrenv_t oldenv;
#endif
	FAR uint32_t *stack;
	pid_t pid;
	int ret;
#ifdef CONFIG_APP_BINARY_SEPARATION
	int binary_idx;
#endif

	/* Sanity checking */

#ifdef CONFIG_DEBUG
	if (!binp || !binp->entrypt || binp->stacksize <= 0) {
		return -EINVAL;
	}
#endif

	binfo("Executing %s\n", binp->filename);

#ifdef CONFIG_APP_BINARY_SEPARATION
	binary_idx = binp->binary_idx;
	binp->uheap = (struct mm_heap_s *)binp->heapstart;
	mm_initialize(binp->uheap, (void *)binp->heapstart + sizeof(struct mm_heap_s), binp->uheap_size);
	mm_add_app_heap_list(binp->uheap, binp->bin_name);

	binfo("------------------------%s Binary Heap Information------------------------\n", binp->bin_name);
	binfo("Start addr = 0x%x, size = %u \n", (void *)binp->heapstart + sizeof(struct mm_heap_s), binp->uheap_size);

	/* The first 4 bytes of the text section of the application must contain a
	pointer to the application's mm_heap object. Here we will store the mm_heap
	pointer to the start of the text section */
	*(uint32_t *)(binp->datastart) = (uint32_t)binp->uheap;
	rtcb = (struct tcb_s *)sched_self();
	rtcb->uheap = (uint32_t)binp->uheap;

	/* Initialize the MPU registers in tcb with suitable protection values */
#ifdef CONFIG_ARM_MPU
	uint8_t nregion = mpu_get_nregion_info(MPU_REGION_APP_BIN);
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	/* Configure text section as RO and executable region */
	mpu_get_register_config_value(&rtcb->mpu_regs[0], nregion - 3, (uintptr_t)binp->alloc[ALLOC_TEXT], binp->textsize, true, true);
	/* Configure ro section as RO and non-executable region */
	mpu_get_register_config_value(&rtcb->mpu_regs[3], nregion - 2, (uintptr_t)binp->alloc[ALLOC_RO], binp->rosize, true, false);
	/* Complete RAM partition will be configured as RW region */
	mpu_get_register_config_value(&rtcb->mpu_regs[6], nregion - 1, (uintptr_t)binp->alloc[ALLOC_DATA], binp->ramsize, false, false);
#else
	/* Complete RAM partition will be configured as RW region */
	mpu_get_register_config_value(&rtcb->mpu_regs[0], nregion - 1, (uintptr_t)binp->ramstart, binp->ramsize, false, true);
#endif
#endif
#endif /* CONFIG_APP_BINARY_SEPARATION */

	/* Allocate a TCB for the new task. */

	tcb = (FAR struct task_tcb_s *)kmm_zalloc(sizeof(struct task_tcb_s));
	if (!tcb) {
		ret = -ENOMEM;
		goto errout_with_appheap;
	}
#if defined(CONFIG_ARCH_ADDRENV) && defined(CONFIG_BUILD_KERNEL)
	/* Instantiate the address environment containing the user heap */

	ret = up_addrenv_select(&binp->addrenv, &oldenv);
	if (ret < 0) {
		berr("ERROR: up_addrenv_select() failed: %d\n", ret);
		goto errout_with_tcb;
	}
#endif

	ret = up_create_stack((FAR struct tcb_s *)tcb, binp->stacksize, TCB_FLAG_TTYPE_TASK);
	if (ret < 0) {
		berr("ERROR: up_create_stack() failed.\n");
		goto errout_with_addrenv;
	}
	stack = tcb->cmn.stack_alloc_ptr;

	/* Initialize the task */

	ret = task_init((FAR struct tcb_s *)tcb, binp->filename, binp->priority, NULL, binp->stacksize, binp->entrypt, binp->argv);
	if (ret < 0) {
		ret = -get_errno();
		berr("task_init() failed: %d\n", ret);
		goto errout_with_stack;
	}

#if defined(CONFIG_DEBUG_MM_HEAPINFO) && defined(CONFIG_APP_BINARY_SEPARATION)
	/* Re-initialize the binary heap alloc list information.
	 * Loading thread uses the binary's heap for loading,
	 * so it saves the alloc data in binary's heap.
	 * But loading thread is not in its binary, exclude those data
	 * for calculating the heap usage per threads.
	 */
	int hashpid = PIDHASH(getpid());
	struct mm_allocnode_s *node;

	binp->uheap->alloc_list[hashpid].pid = HEAPINFO_INIT_INFO;
	binp->uheap->alloc_list[hashpid].curr_alloc_size = 0;
	binp->uheap->alloc_list[hashpid].num_alloc_free = 0;

	node = (struct mm_allocnode_s *)((void *)stack - SIZEOF_MM_ALLOCNODE);
	node->pid = (-1) * (tcb->cmn.pid);
#endif

	/* We can free the argument buffer now.
	 * REVISIT:  It is good to free up memory as soon as possible, but
	 * unfortunately here 'binp' is 'const'.  So to do this properly, we will
	 * have to make some more extensive changes.
	 */

	binfmt_freeargv((FAR struct binary_s *)binp);

	/* Note that tcb->flags are not modified.  0=normal task */
	/* tcb->flags |= TCB_FLAG_TTYPE_TASK; */

#if defined(CONFIG_ARCH_ADDRENV) && defined(CONFIG_BUILD_KERNEL)
	/* Allocate the kernel stack */

	ret = up_addrenv_kstackalloc(&tcb->cmn);
	if (ret < 0) {
		berr("ERROR: up_addrenv_select() failed: %d\n", ret);
		goto errout_with_tcbinit;
	}
#endif

#if defined(CONFIG_BUILD_KERNEL) && defined(CONFIG_MM_SHM)
	/* Initialize the shared memory virtual page allocator */

	ret = shm_group_initialize(tcb->cmn.group);
	if (ret < 0) {
		berr("ERROR: shm_group_initialize() failed: %d\n", ret);
		goto errout_with_tcbinit;
	}
#endif

#ifdef CONFIG_PIC
	/* Add the D-Space address as the PIC base address.  By convention, this
	 * must be the first allocated address space.
	 */

	tcb->cmn.dspace = binp->alloc[ALLOC_TEXT];

	/* Re-initialize the task's initial state to account for the new PIC base */

	up_initial_state(&tcb->cmn);
#endif

#ifdef CONFIG_ARCH_ADDRENV
	/* Assign the address environment to the new task group */

	ret = up_addrenv_clone(&binp->addrenv, &tcb->cmn.group->tg_addrenv);
	if (ret < 0) {
		berr("ERROR: up_addrenv_clone() failed: %d\n", ret);
		goto errout_with_tcbinit;
	}

	/* Mark that this group has an address environment */

	tcb->cmn.group->tg_flags |= GROUP_FLAG_ADDRENV;
#endif

#ifdef CONFIG_BINFMT_CONSTRUCTORS
	/* Setup a start hook that will execute all of the C++ static constructors
	 * on the newly created thread.  The struct binary_s must persist at least
	 * until the new task has been started.
	 */

	if (binp->nctors > 0) {
		task_starthook(tcb, exec_ctors, (FAR void *)binp);
	}
#endif

	/* Get the assigned pid before we start the task */

	pid = tcb->cmn.pid;

#ifdef CONFIG_APP_BINARY_SEPARATION
	rtcb->uheap = 0;

	/* Store the address of the applications userspace object in the tcb  */
	/* The app's userspace object will be found at an offset of 4 bytes from the start of the binary */
	tcb->cmn.uspace = (uint32_t)binp->alloc[ALLOC_TEXT] + 4;
	tcb->cmn.uheap = (uint32_t)binp->uheap;

#ifdef CONFIG_BINARY_MANAGER
#ifdef CONFIG_SUPPORT_COMMON_BINARY
	tcb->cmn.app_id = binp->binary_idx;
#endif

	/* Set task name as binary name */
	strncpy(tcb->cmn.name, binp->bin_name, CONFIG_TASK_NAME_SIZE);
	tcb->cmn.name[CONFIG_TASK_NAME_SIZE] = '\0';

	tcb->cmn.group->tg_binidx = binary_idx;
	binary_manager_add_binlist(&tcb->cmn);

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
	tcb->cmn.sigprocmask = NULL_SIGNAL_SET;
#endif
#endif /* CONFIG_APP_BINARY_SEPARATION */

	/* Set up to unload the module when the task exists. */

	tcb->bininfo = binp;

	/* Then activate the task at the provided priority */

	ret = task_activate((FAR struct tcb_s *)tcb);
	if (ret < 0) {
		ret = -get_errno();
		berr("task_activate() failed: %d\n", ret);
		goto errout_with_tcbinit;
	}

#if defined(CONFIG_ARCH_ADDRENV) && defined(CONFIG_BUILD_KERNEL)
	/* Restore the address environment of the caller */

	ret = up_addrenv_restore(&oldenv);
	if (ret < 0) {
		berr("ERROR: up_addrenv_select() failed: %d\n", ret);
		goto errout_with_tcbinit;
	}
#endif
	binfo("%s loaded @ 0x%08x and running with pid = %d\n", binp->filename, binp->alloc[ALLOC_TEXT], pid);

	return (int)pid;

errout_with_appheap:
#ifdef CONFIG_APP_BINARY_SEPARATION
	mm_remove_app_heap_list(binp->uheap);
#endif
errout_with_tcbinit:
	if (tcb != NULL) {
#ifdef CONFIG_BINARY_MANAGER
		BIN_ID(binary_idx) = -1;
		BIN_STATE(binary_idx) = BINARY_INACTIVE;
		BIN_LOADVER(binary_idx) = 0;
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
		BIN_LOADINFO(binary_idx) = NULL;
#endif
		binary_manager_remove_binlist(&tcb->cmn);
#endif
		sched_releasetcb(&tcb->cmn, TCB_FLAG_TTYPE_TASK);
	}
	return ret;

errout_with_stack:
	kumm_free(stack);

errout_with_addrenv:
#if defined(CONFIG_ARCH_ADDRENV) && defined(CONFIG_BUILD_KERNEL)
	(void)up_addrenv_restore(&oldenv);

errout_with_tcb:
#endif
	kmm_free(tcb);
	return ret;
}

#endif							/* CONFIG_BINFMT_ENABLE */
