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
#include <tinyara/mpu.h>

#ifdef CONFIG_SAVE_BIN_SECTION_ADDR
#include "libelf/libelf.h"
#endif

#include "binfmt.h"
#include "binary_manager/binary_manager.h"

#ifdef CONFIG_BINFMT_ENABLE

#include <tinyara/mm/mm.h>

#ifdef CONFIG_SUPPORT_COMMON_BINARY
struct binary_s *g_lib_binp;
uint32_t *g_umm_app_id;
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
 *               to be loaded. load_attr will be NULL while loading a library.
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
	FAR struct binary_s *bin = NULL;
	int pid;
	int errcode;
	int ret;
#if (defined(CONFIG_SUPPORT_COMMON_BINARY) && (defined(CONFIG_ARMV7M_MPU) || defined(CONFIG_ARMV8M_MPU)))
	uint32_t com_bin_mpu_regs[MPU_REG_NUMBER * MPU_NUM_REGIONS];	/* We need 3 register values to configure each MPU region */
#endif

	/* Sanity check */
	if (load_attr && load_attr->bin_size <= 0) {
		berr("ERROR: Invalid file length!\n");
		errcode = -EINVAL;
		goto errout;
	}

#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME

#ifdef CONFIG_SUPPORT_COMMON_BINARY
	if (load_attr) {
		bin = load_attr->binp;
	} else {
		bin = g_lib_binp;
	}
#else
	bin = load_attr->binp;
#endif
	/* If we find a non-null value for bin, it means that
	 * we are in a reload scenario.
	 */
	if (bin) {
		if (!bin->data_backup) {
			errcode = -EINVAL;
			berr("ERROR: Failed to find copy of data section from previous load\n");
			goto errout_with_bin;
		}

		memcpy((void *)bin->datastart, (const void *)bin->data_backup, bin->datasize);
		memset((void *)bin->bssstart, 0, bin->bsssize);
		bin->reload = false;
	} else {
#endif

		/* Allocate the load information */

		bin = (FAR struct binary_s *)kmm_zalloc(sizeof(struct binary_s));
		if (!bin) {
			berr("ERROR: Failed to allocate binary_s\n");
			errcode = -ENOMEM;
			goto errout_with_bin;
		}

		/* Initialize the binary structure */

		bin->filename = filename;
		if (load_attr) {
			bin->exports = NULL;
			bin->nexports = 0;
			bin->filelen = load_attr->bin_size;
			bin->offset = load_attr->offset;
			bin->stacksize = load_attr->stack_size;
			bin->priority = load_attr->priority;
			bin->compression_type = load_attr->compression_type;
			bin->binary_idx = binary_idx;
			bin->bin_ver = load_attr->bin_ver;
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
			strncpy(bin->bin_name, load_attr->bin_name, BIN_NAME_MAX);
#else
			bin->bin_name = load_attr->bin_name;
#endif
			bin->ramsize = load_attr->ram_size;
		} else {
#ifdef CONFIG_SUPPORT_COMMON_BINARY
			bin->islibrary = true;
			g_lib_binp = bin;
#endif
		}

		/* Load the module into memory */
		ret = load_module(bin);
		if (ret < 0) {
			errcode = ret;
			berr("ERROR: Failed to load program '%s': %d\n", filename, errcode);
			goto errout_with_bin;
		}

#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
		if (!bin->data_backup) {
			errcode = -EINVAL;
			berr("ERROR: data section backup address not initialized\n");
			goto errout_with_bin;
		}

		memcpy((void *)bin->data_backup, (const void *)bin->datastart, bin->datasize);
	}
#endif

#ifdef CONFIG_SUPPORT_COMMON_BINARY
	if (bin->islibrary) {
		g_umm_app_id = (uint32_t *)(bin->datastart + 4);
#ifdef CONFIG_SAVE_BIN_SECTION_ADDR
		elf_save_bin_section_addr(bin);
#endif

#if (defined(CONFIG_ARMV7M_MPU) || defined(CONFIG_ARMV8M_MPU))
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
		uint8_t nregion = mpu_get_nregion_info(MPU_REGION_COMMON_BIN);
		/* Get MPU register values for MPU regions */
		mpu_get_register_config_value(&com_bin_mpu_regs[0], nregion - 3, (uintptr_t)bin->alloc[ALLOC_TEXT], bin->textsize, true,  true);
		mpu_get_register_config_value(&com_bin_mpu_regs[3], nregion - 2, (uintptr_t)bin->alloc[ALLOC_RO],   bin->rosize,   true,  false);
		mpu_get_register_config_value(&com_bin_mpu_regs[6], nregion - 1, (uintptr_t)bin->alloc[ALLOC_DATA], bin->ramsize,  false, false);
#else
		mpu_get_register_config_value(&com_bin_mpu_regs[0], nregion - 1, (uintptr_t)bin->ramstart,          bin->ramsize,  false, true);
#endif
		/* Set MPU register values to real MPU h/w */
		for (int i = 0; i < MPU_REG_NUMBER * MPU_NUM_REGIONS; i += MPU_REG_NUMBER) {
			up_mpu_set_register(&com_bin_mpu_regs[i]);
		}
#endif

		return OK;
	}
	/* If we support common binary, then we need to place a pointer to the app's heap object
	 * into the heap table which is present at the start of the common library data section
	 */
	uint32_t *heap_table = (uint32_t *)(g_lib_binp->datastart + 8);
	heap_table[binary_idx] = bin->heapstart;
#endif

	/* Start the module */
	pid = exec_module(bin);
	if (pid < 0) {
		errcode = pid;
		berr("ERROR: Failed to execute program '%s': %d\n", filename, errcode);
		goto errout_with_unload;
	}
#ifdef CONFIG_SAVE_BIN_SECTION_ADDR
	elf_save_bin_section_addr(bin);
#endif

	/* Print Binary section address & size details */

	binfo("[%s] text    start addr =  0x%x  size = %u\n", bin->bin_name, (uint32_t)bin->alloc[ALLOC_TEXT], bin->textsize);
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	binfo("[%s] rodata  start addr =  0x%x  size = %u\n", bin->bin_name, (uint32_t)bin->alloc[ALLOC_RO], bin->rosize);
	binfo("[%s] data    start addr =  0x%x  size = %u\n", bin->bin_name, (uint32_t)bin->datastart, bin->datasize);
	binfo("[%s] bss     start addr =  0x%x  size = %u\n", bin->bin_name, (uint32_t)bin->bssstart, bin->bsssize);
#endif

	return pid;

errout_with_unload:
	(void)unload_module(bin);
errout_with_bin:
	kmm_free(bin);
#ifdef CONFIG_SUPPORT_COMMON_BINARY
	g_lib_binp = NULL;
#endif
errout:
	set_errno(errcode);
	return ERROR;

}
#endif							/* CONFIG_BINARY_MANAGER */
#endif							/* CONFIG_BINFMT_ENABLE */
