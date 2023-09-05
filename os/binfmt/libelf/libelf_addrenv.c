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
 * os/binfmt/libelf/libelf_addrenv.c
 *
 *   Copyright (C) 2012 Gregory Nutt. All rights reserved.
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

#include <errno.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/kmalloc.h>
#include <tinyara/mm/mm.h>
#include "libelf.h"
#include "binfmt_arch_apis.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Constant Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
static int allocateregions(FAR struct elf_loadinfo_s *loadinfo)
{
	size_t sizes[NUM_APP_REGIONS] = {loadinfo->binp->sizes[BIN_TEXT], loadinfo->binp->sizes[BIN_RO], loadinfo->binp->ramsize};
	uintptr_t *allocs[NUM_APP_REGIONS] = {&loadinfo->binp->sections[BIN_TEXT], &loadinfo->binp->sections[BIN_RO], &loadinfo->binp->sections[BIN_DATA]};
	int count = 0;
	int i;
	for (i = 0; i < CONFIG_KMM_REGIONS; i++) {
		if (kregionx_heap_idx[i] == CONFIG_HEAP_INDEX_LOADED_APP) {
			count++;
		}
	}
	if (count != 1) {
		lldbg("ERROR: Expected to have exactly one heap region for apps, but found %d regions\n", count);
		ASSERT(0);
	}

#if defined(CONFIG_BINFMT_SECTION_UNIFIED_MEMORY)
	uint32_t totalsize = sizes[0] + sizes[1] + sizes[2];
	size_t tmpsz;
	uintptr_t *tmpalloc;
	uint8_t j;

	for (i = 0; i < NUM_APP_REGIONS; i++) {
		for (j = 0; j < NUM_APP_REGIONS - (i + 1); j++) {
			if (sizes[j] < sizes[j + 1]) {
				tmpsz = sizes[j];
				sizes[j] = sizes[j + 1];
				sizes[j + 1] = tmpsz;

				tmpalloc = allocs[j];
				allocs[j] = allocs[j + 1];
				allocs[j + 1] = tmpalloc;
			}
		}
	}

	*allocs[0] = (uintptr_t)binfmt_arch_allocate_section(totalsize);
	if (*allocs[0] == (uintptr_t)NULL) {
		return -ENOMEM;
	}
	loadinfo->binp->ramstart = (uint32_t)*allocs[0];
	*allocs[1] = *allocs[0] + sizes[0];
	*allocs[2] = *allocs[1] + sizes[1];

#else
	int region_idx;
	for (region_idx = 0; region_idx < NUM_APP_REGIONS; region_idx++) {
		*allocs[region_idx] = (uintptr_t)binfmt_arch_allocate_section(sizes[region_idx]);
		if (*allocs[region_idx] == (uintptr_t)NULL) {
			return -ENOMEM;
		}
	}
#endif
	return 0;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: elf_addrenv_alloc
 *
 * Description:
 *   Allocate memory for the ELF sections.
 *
 * Input Parameters:
 *   loadinfo - Load state information
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ****************************************************************************/

int elf_addrenv_alloc(FAR struct elf_loadinfo_s *loadinfo)
{
	/* Allocate memory to hold the ELF image */

#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	/* Total size allocated for data section should include data, bss and heap */
	uint32_t datamemsize = loadinfo->binp->sizes[BIN_DATA] + loadinfo->binp->sizes[BIN_BSS] + loadinfo->binp->ramsize;
	uint32_t rosize = loadinfo->binp->sizes[BIN_RO];
	loadinfo->binp->sizes[BIN_RO] += loadinfo->binp->sizes[BIN_DATA];

	loadinfo->binp->sizes[BIN_TEXT] = binfmt_arch_align_mem(loadinfo->binp->sizes[BIN_TEXT]);
	loadinfo->binp->sizes[BIN_RO] = binfmt_arch_align_mem(loadinfo->binp->sizes[BIN_RO]);
	datamemsize = binfmt_arch_align_mem(datamemsize);
	
	loadinfo->binp->ramsize = datamemsize;

	if (allocateregions(loadinfo) < 0) {
		berr("ERROR: failed to allocate memory\n");
		return -ENOMEM;
	}

	/* Data section back up will be stored at the end of RO section. Adjust the size accordingly */
	loadinfo->binp->data_backup = loadinfo->binp->sections[BIN_RO] + rosize;
	loadinfo->binp->sizes[BIN_HEAP] = datamemsize - loadinfo->binp->sizes[BIN_DATA] - loadinfo->binp->sizes[BIN_BSS] - sizeof(struct mm_heap_s);
#else
	/* ramsize may be zero in case of loading library since we dont have header for library */
	if (loadinfo->binp->ramsize == 0) {
		loadinfo->binp->ramsize = loadinfo->binp->sizes[BIN_TEXT] + loadinfo->binp->sizes[BIN_DATA] + loadinfo->binp->sizes[BIN_BSS];
	}

	/* Allocate the RAM partition to load the app into */
	loadinfo->binp->ramsize = binfmt_arch_align_mem(loadinfo->binp->ramsize);
	loadinfo->binp->ramstart = (uint32_t)binfmt_arch_allocate_section(loadinfo->binp->ramsize);

	if (!loadinfo->binp->ramstart) {
		berr("ERROR: Failed to allocate RAM partition\n");
		return -ENOMEM;
	}

	loadinfo->binp->sections[BIN_TEXT] = loadinfo->binp->ramstart;
	loadinfo->binp->sections[BIN_DATA] = loadinfo->binp->sections[BIN_TEXT] + loadinfo->binp->sizes[BIN_TEXT];
	loadinfo->binp->sizes[BIN_HEAP] = loadinfo->binp->ramsize 
					- (loadinfo->binp->sizes[BIN_TEXT] + loadinfo->binp->sizes[BIN_DATA] + loadinfo->binp->sizes[BIN_BSS]) 
					- sizeof(struct mm_heap_s);
#endif
	loadinfo->binp->sections[BIN_HEAP] = loadinfo->binp->sections[BIN_DATA] + loadinfo->binp->sizes[BIN_DATA] + loadinfo->binp->sizes[BIN_BSS];

	loadinfo->binp->sections[BIN_BSS] = loadinfo->binp->sections[BIN_DATA] + loadinfo->binp->sizes[BIN_DATA];
	memset(loadinfo->binp->sections[BIN_BSS], 0, loadinfo->binp->sizes[BIN_BSS]);

	if (!loadinfo->binp->sections[BIN_TEXT]) {
		berr("ERROR: Failed to allocate text section (size = %u)\n", loadinfo->binp->sizes[BIN_TEXT]);
		return -ENOMEM;
	}

	if (!loadinfo->binp->sections[BIN_DATA]) {
		berr("ERROR: Failed to allocate data section (size = %u)\n", loadinfo->binp->ramsize);
		return -ENOMEM;
	}

#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	if (!loadinfo->binp->sections[BIN_RO]) {
		berr("ERROR: Failed to allocate ro section (size = %u)\n", loadinfo->binp->sizes[BIN_RO]);
		return -ENOMEM;
	}
#endif
	return OK;
}

/****************************************************************************
 * Name: elf_addrenv_free
 *
 * Description:
 *   Release the address environment previously created by
 *   elf_addrenv_alloc().  This function  is called only under certain error
 *   conditions after the module has been loaded but not yet started.
 *   After the module has been started, the address environment will
 *   automatically be freed when the module exits.
 *
 * Input Parameters:
 *   loadinfo - Load state information
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

void elf_addrenv_free(FAR struct elf_loadinfo_s *loadinfo)
{
	if (loadinfo->binp->sections[BIN_TEXT]) {
		kmm_free((void *)(loadinfo->binp->sections[BIN_TEXT]));
		loadinfo->binp->sections[BIN_TEXT] = (int)NULL;
	}
	if (loadinfo->binp->sections[BIN_DATA]) {
		kmm_free((void *)(loadinfo->binp->sections[BIN_DATA]));
		loadinfo->binp->sections[BIN_DATA] = (int)NULL;
	}
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	if (loadinfo->binp->sections[BIN_RO]) {
		kmm_free((void *)(loadinfo->binp->sections[BIN_RO]));
		loadinfo->binp->sections[BIN_RO] = (int)NULL;
	}
#endif
}
