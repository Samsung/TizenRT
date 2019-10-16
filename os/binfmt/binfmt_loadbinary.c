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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <string.h>
#include <debug.h>
#include <errno.h>
#if defined(CONFIG_BINARY_MANAGER) && !defined(CONFIG_DISABLE_SIGNALS)
#include <signal.h>
#endif

#include <tinyara/kmalloc.h>
#include <tinyara/sched.h>
#include <tinyara/binfmt/binfmt.h>
#include <tinyara/binary_manager.h>

#include "binfmt.h"
#include "binary_manager/binary_manager.h"

#ifdef CONFIG_BINFMT_ENABLE

#ifdef CONFIG_ARMV7M_MPU
extern uint32_t g_app_mpu_region;
extern void mpu_user_extsram_context(uint32_t region, uintptr_t base, size_t size, uint32_t *regs);
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: load_binary
 *
 * Description:
 *   This is a convenience function that wraps load_module and exec_module into
 *   one call.  If CONFIG_BINFMT_LOADABLE is defined, this function will
 *   schedule to unload the module when task exits.
 *
 * Input Parameters:
 *   filename  - The path to the program to be executed. If
 *               CONFIG_LIB_ENVPATH is defined in the configuration, then
 *               this may be a relative path from the current working
 *               directory. Otherwise, path must be the absolute path to the
 *               program.
 *
 *   load_attr - This structure contains the information that the binary
 *               to be loaded.
 *
 * Returned Value:
 *   This is an end-user function, so it follows the normal convention:
 *   It returns the PID of the exec'ed module.  On failure, it returns
 *   -1 (ERROR) and sets errno appropriately.
 *
 ****************************************************************************/
#ifdef CONFIG_BINARY_MANAGER
int load_binary(int binary_idx, FAR const char *filename, load_attr_t *load_attr)
{
	FAR struct binary_s *bin;
	int pid;
	int errcode;
	int ret;

	/* Sanity check */
	if (load_attr->bin_size <= 0) {
		berr("ERROR: Invalid file length!\n");
		errcode = EINVAL;
		goto errout;
	}

#ifdef CONFIG_APP_BINARY_SEPARATION
	/* Allocate the RAM partition to load the app into */
	uint32_t *start_addr;
	uint32_t size = load_attr->ram_size;
	struct tcb_s *tcb;

	if (mm_allocate_ram_partition(&start_addr, &size, load_attr->bin_name) < 0) {
		berr("ERROR: Failed to allocate RAM partition\n");
		errcode = ENOMEM;
		goto errout;
	}
#endif

	/* Allocate the load information */

	bin = (FAR struct binary_s *)kmm_zalloc(sizeof(struct binary_s));
	if (!bin) {
		berr("ERROR: Failed to allocate binary_s\n");
		errcode = ENOMEM;
		goto err_free_partition;
	}

	/* Initialize the binary structure */

	bin->filename = filename;
	bin->exports = NULL;
	bin->nexports = 0;
	bin->filelen = load_attr->bin_size;
	bin->offset = load_attr->offset;
	bin->stacksize = load_attr->stack_size;
	bin->priority = load_attr->priority;
#ifdef CONFIG_APP_BINARY_SEPARATION
	bin->uheap = (struct mm_heap_s *)start_addr;
	bin->uheap_size = size;
#endif
	bin->compression_type = load_attr->compression_type;

	/* Load the module into memory */

	ret = load_module(bin);
	if (ret < 0) {
		errcode = -ret;
		berr("ERROR: Failed to load program '%s': %d\n", filename, errcode);
		goto errout_with_bin;
	}

	/* Disable pre-emption so that the executed module does
	 * not return until we get a chance to connect the on_exit
	 * handler.
	 */

	sched_lock();

#ifdef CONFIG_APP_BINARY_SEPARATION
	/* The first 4 bytes of the text section of the application must contain a
	pointer to the application's mm_heap object. Here we will store the mm_heap
	pointer to the start of the text section */
	*(uint32_t *)(bin->alloc[0]) = (uint32_t)start_addr;
	tcb = (struct tcb_s *)sched_self();
	tcb->ram_start = (uint32_t)start_addr;

	/* Initialize the MPU registers in tcb with suitable protection values */
#ifdef CONFIG_ARMV7M_MPU
	mpu_user_extsram_context(g_app_mpu_region, (uintptr_t)start_addr, size, tcb->mpu_regs);
#endif

#endif

	/* Then start the module */
	pid = exec_module(bin);
	if (pid < 0) {
		errcode = -pid;
		berr("ERROR: Failed to execute program '%s': %d\n", filename, errcode);
		goto errout_with_lock;
	}

#ifdef CONFIG_APP_BINARY_SEPARATION
	tcb->ram_start = 0;
	tcb = sched_gettcb(pid);
	if (tcb == NULL) {
		errcode = ESRCH;
		goto errout_with_lock;
	}
	tcb->ram_start = (uint32_t)start_addr;
	tcb->ram_size = size;
	/* Set task name as binary name */
	strncpy(tcb->name, load_attr->bin_name, CONFIG_TASK_NAME_SIZE);
	tcb->name[CONFIG_TASK_NAME_SIZE] = '\0';
#endif

#if defined(CONFIG_BINARY_MANAGER) && !defined(CONFIG_DISABLE_SIGNALS)
	/* Clean the signal mask of loaded task because it inherits signal mask from parent task, binary manager. */
	tcb->sigprocmask = NULL_SIGNAL_SET;
#endif

#ifdef CONFIG_BINFMT_LOADABLE
	/* Set up to unload the module (and free the binary_s structure)
	 * when the task exists.
	 */

	ret = group_exitinfo(pid, bin);
	if (ret < 0) {
		berr("ERROR: Failed to schedule unload '%s': %d\n", filename, ret);
	}
#else
	/* Free the binary_s structure here */

	binfmt_freeargv(bin);
	kmm_free(bin);

	/* TODO: How does the module get unloaded in this case? */
#endif

	binfo("%s loaded @ 0x%08x and running with pid = %d\n", bin->filename, bin->alloc[0], pid);

	/* Update binary id and state for fault handling before unlocking scheduling */

	BIN_ID(binary_idx) = pid;
	BIN_STATE(binary_idx) = BINARY_LOADING_DONE;

	sched_unlock();
	return pid;

errout_with_lock:
	sched_unlock();
	(void)unload_module(bin);
errout_with_bin:
	kmm_free(bin);
err_free_partition:
#ifdef CONFIG_APP_BINARY_SEPARATION
	mm_free_ram_partition((uint32_t)start_addr);
#endif
errout:
	set_errno(errcode);
	return ERROR;

}
#endif							/* CONFIG_BINARY_MANAGER */
#endif							/* CONFIG_BINFMT_ENABLE */
