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
 * os/binfmt/binfmt_exit.c
 *
 *   Copyright (C) 2013, 2016-2018 Gregory Nutt. All rights reserved.
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
#include <assert.h>
#include <debug.h>
#ifdef CONFIG_APP_BINARY_SEPARATION
#include <queue.h>
#ifdef CONFIG_SAVE_BIN_SECTION_ADDR
#include "libelf/libelf.h"
#endif
#endif

#include <tinyara/mm/mm.h>
#include <tinyara/kmalloc.h>
#include <tinyara/binfmt/binfmt.h>

#include "binfmt.h"

#ifdef CONFIG_BINFMT_LOADABLE
#ifdef CONFIG_APP_BINARY_SEPARATION
extern volatile sq_queue_t g_delayed_kufree;
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: binfmt_exit
 *
 * Description:
 *   This function may be called when a tasked loaded into RAM exits.
 *   This function will unload the module when the task exits and reclaim
 *   all resources used by the module.
 *
 * Input Parameters:
 *   bin - This structure must have been allocated with kmm_malloc() and must
 *         persist until the task unloads
 *
 * Returned Value:
 *   This is a internal function so it follows the convention that
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int binfmt_exit(FAR struct binary_s *bin)
{
	int ret;
#ifdef CONFIG_APP_BINARY_SEPARATION
	irqstate_t flags;
	FAR void *address;
	uint32_t uheap_start;
	uint32_t uheap_end;
#endif
	DEBUGASSERT(bin != NULL);

	/* Unload the module */

	ret = unload_module(bin);
	if (ret < 0) {
		berr("ERROR: unload_module() failed: %d\n", ret);
	}

#ifdef CONFIG_SAVE_BIN_SECTION_ADDR
	elf_delete_bin_section_addr(bin);
#endif

#ifdef CONFIG_APP_BINARY_SEPARATION
	uheap_start = (uint32_t)bin->uheap;
	uheap_end = uheap_start + bin->uheap_size;

	/* Remove resources which in binary to be unloaded from delayed deallocation. */
	address = sq_peek(&g_delayed_kufree);
	while (address) {
		mllvdbg("Remove addr %p from deplaed kufree, uheap (%p, %p)\n", address, uheap_start, uheap_end);
		if (uheap_start <= (uint32_t)address && (uint32_t)address <= uheap_end) {
			flags = irqsave();
			sq_rem((FAR sq_entry_t *)address, (FAR sq_queue_t *)&g_delayed_kufree);
			irqrestore(flags);
		}
		address = (FAR void *)sq_next((FAR sq_entry_t *)address);
	}
	mm_disable_app_heap_list(bin->uheap);
#endif
	/* Free the load structure */

#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	if (!bin->reload) {
#endif
		mm_remove_app_heap_list(bin->uheap);
		/* Free the RAM partition into which this app was loaded */
		kmm_free((void *)bin->ramstart);
		kmm_free(bin);
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	}
#endif

	return ret;
}

#endif							/* CONFIG_BINFMT_LOADABLE */
