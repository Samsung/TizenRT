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
 * os/binfmt/binfmt_unloadmodule.c
 *
 *   Copyright (C) 2009, 2012-2013, 2017 Gregory Nutt. All rights reserved.
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

#include <sys/mman.h>
#include <stdlib.h>
#include <sched.h>
#include <debug.h>
#include <errno.h>

#include <tinyara/kmalloc.h>
#include <tinyara/binfmt/binfmt.h>

#include "binfmt.h"
#include "libelf/libelf.h"

#ifdef CONFIG_BINFMT_ENABLE

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: exec_dtors
 *
 * Description:
 *   Execute C++ static destructors.
 *
 * Input Parameters:
 *   binp - Load state information
 *
 * Returned Value:
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

#ifdef CONFIG_BINFMT_CONSTRUCTORS
static inline int exec_dtors(FAR struct binary_s *binp)
{
	binfmt_dtor_t *dtor = binp->dtors;
#ifdef CONFIG_ARCH_ADDRENV
	save_addrenv_t oldenv;
	int ret;
#endif
	int i;

	/* Instantiate the address environment containing the destructors */

#ifdef CONFIG_ARCH_ADDRENV
	ret = up_addrenv_select(&binp->addrenv, &oldenv);
	if (ret < 0) {
		berr("ERROR: up_addrenv_select() failed: %d\n", ret);
		return ret;
	}
#endif

	/* Execute each destructor */

	for (i = 0; i < binp->ndtors; i++) {
		binfo("Calling dtor %d at %p\n", i, (FAR void *)dtor);

		(*dtor)();
		dtor++;
	}

	/* Restore the address environment */

#ifdef CONFIG_ARCH_ADDRENV
	return up_addrenv_restore(&oldenv);
#else
	return OK;
#endif
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: unload_module
 *
 * Description:
 *   Unload a (non-executing) module from memory.  If the module has
 *   been started (via exec_module) and has not exited, calling this will
 *   be fatal.
 *
 *   However, this function must be called after the module exits.  How
 *   this is done is up to your logic.  Perhaps you register it to be
 *   called by on_exit()?
 *
 * Returned Value:
 *   This is a internal function so it follows the convention that
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int unload_module(FAR struct binary_s *binp)
{
	int ret;

	if (binp) {
		/* Perform any format-specific unload operations */

		if (binp->unload) {
			ret = binp->unload(binp);
			if (ret < 0) {
				berr("binp->unload() failed: %d\n", ret);
				return ret;
			}
		}
#ifdef CONFIG_BINFMT_CONSTRUCTORS
		/* Execute C++ destructors */

		ret = exec_dtors(binp);
		if (ret < 0) {
			berr("exec_ctors() failed: %d\n", ret);
			return ret;
		}
#endif

		/* Free any allocated argv[] strings */

		binfmt_freeargv(binp);

		/* Unmap mapped address spaces */

		if (binp->mapped) {
			binfo("Unmapping address space: %p\n", binp->mapped);

			munmap(binp->mapped, binp->mapsize);
		}

		/* Free allocated address spaces */

#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
		if (binp->reload != true) {
		/* If reload is true, reserved loading section information is used for reloading, so there is no need to free each loading section's memory.
		 * If reload is false, it means normal termination, then free to each section's memory.
		 */

#ifdef CONFIG_BINFMT_SECTION_UNIFIED_MEMORY
			/* For MPU restrictions, binary loader allocates one big memory block enough to contains each loading sections
			 * and assigns each sections start address based on the size.
			 * For free the each section, find the start address of big memory block.
			 */
			void *start_addr = elf_find_start_section_addr(binp);
			binfo("Freeing section memory: %p\n", start_addr);
			kmm_free(start_addr);
#else
			/* Each loading section is allocated respectively.
			 * They need to be freed each.
			 */
			int section_idx;
			for (section_idx = 0; section_idx < ALLOC_MAX; section_idx++) {
				if (binp->alloc[section_idx]) {
					binfo("Freeing alloc[%d]: %p\n", section_idx, binp->alloc[section_idx]);
					kmm_free((FAR void *)binp->alloc[section_idx]);
				}
			}

#endif
		}
#else
		/* Whole loading sections are in one memory block, so free the first allocated memory is enough. */
		binfo("Freeing : %p\n", binp->alloc[0]);
		kmm_free((FAR void *)binp->alloc[0]);
#endif
		/* Notice that the address environment is not destroyed.  This should
		 * happen automatically when the task exits.
		 */
	}

	return OK;
}

#endif							/* CONFIG_BINFMT_ENABLE */
