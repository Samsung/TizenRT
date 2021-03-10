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
#include <stdbool.h>
#include <stdint.h>
#ifdef CONFIG_SUPPORT_COMMON_BINARY
#include <tinyara/binfmt/binfmt.h>
#endif

#include "up_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
extern uint32_t _stext;
extern uint32_t _etext;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _ram_kernel_text_start;
extern uint32_t _ram_kernel_text_end;

bool is_kernel_text_space(void *addr)
{
	/* Check if the address lies in the kernel text section range */
	if ((addr >= (void *)&_stext && addr <= (void *)&_etext) ||
		(addr >= (void *)&_ram_kernel_text_start && addr <= (void *)&_ram_kernel_text_end)) {
		return true;
	}
	return false;
}

#ifdef CONFIG_BUILD_PROTECTED
bool is_kernel_space(void *addr)
{
	if ((is_kernel_text_space(addr)) || (addr >= (void *)&_sdata && addr <= (void *)&_ebss)) {
		return true;
	}
	return false;
}

#ifdef CONFIG_SUPPORT_COMMON_BINARY
bool is_common_library_space(void *addr)
{
	if (g_lib_binp && addr >= (void *)g_lib_binp->alloc[ALLOC_TEXT] && addr <= (void *)(g_lib_binp->alloc[ALLOC_TEXT] + g_lib_binp->textsize)) {
		return true;
	}
	return false;
}
#endif							/* CONFIG_SUPPORT_COMMON_BINARY */
#endif							/* CONFIG_BUILD_PROTECTED */
