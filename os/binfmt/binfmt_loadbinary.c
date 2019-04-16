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

#include <tinyara/kmalloc.h>
#include <tinyara/sched.h>
#include <tinyara/binfmt/binfmt.h>

#include "binfmt.h"

#ifndef CONFIG_BINFMT_DISABLE

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
 *   filename - The path to the program to be executed. If
 *              CONFIG_LIB_ENVPATH is defined in the configuration, then
 *              this may be a relative path from the current working
 *              directory. Otherwise, path must be the absolute path to the
 *              program.
 *
 *   binsize  - The size of ELF binary to be loaded, if this value is less than
 *              will return with invalid parameter.
 *
 *   offset   - The offset from which ELF binary has to be read in MTD partition.
 *
 * Returned Value:
 *   This is an end-user function, so it follows the normal convention:
 *   It returns the PID of the exec'ed module.  On failure, it returns
 *   -1 (ERROR) and sets errno appropriately.
 *
 ****************************************************************************/
int load_binary(FAR const char *filename, size_t binsize, size_t offset)
{
	FAR struct binary_s *bin;
	int pid;
	int errcode;
	int ret;

	/* Sanity check */
	if (binsize <= 0) {
		berr("ERROR: Invalid file length!\n");
		errcode = EINVAL;
		goto errout;
	}

	/* Allocate the load information */

	bin = (FAR struct binary_s *)kmm_zalloc(sizeof(struct binary_s));
	if (!bin) {
		berr("ERROR: Failed to allocate binary_s\n");
		errcode = ENOMEM;
		goto errout;
	}

	/* Initialize the binary structure */

	bin->filename = filename;
	bin->exports = NULL;
	bin->nexports = 0;
	bin->filelen = binsize;
	bin->offset = offset;

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

	/* Then start the module */

	pid = exec_module(bin);
	if (pid < 0) {
		errcode = -pid;
		berr("ERROR: Failed to execute program '%s': %d\n", filename, errcode);
		goto errout_with_lock;
	}

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

	sched_unlock();
	return pid;

errout_with_lock:
	sched_unlock();
	(void)unload_module(bin);
errout_with_bin:
	kmm_free(bin);
errout:
	set_errno(errcode);
	return ERROR;

}

#endif							/* !CONFIG_BINFMT_DISABLE */
