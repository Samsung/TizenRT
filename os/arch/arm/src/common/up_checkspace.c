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
extern uint32_t _stext_flash;
extern uint32_t _etext_flash;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
#ifdef CONFIG_ARCH_HAVE_RAM_KERNEL_TEXT
extern uint32_t _stext_ram;
extern uint32_t _etext_ram;
#endif

bool is_kernel_text_space(void *addr)
{
	/* Check if the address lies in the kernel text section range */
	if ((addr >= (void *)&_stext_flash && addr <= (void *)&_etext_flash)
#ifdef CONFIG_ARCH_HAVE_RAM_KERNEL_TEXT
		|| (addr >= (void *)&_stext_ram && addr <= (void *)&_etext_ram)
#endif
	   ) {
		return true;
	}
	return false;
}

bool is_kernel_data_space(void *addr)
{
	/* Check if the address lies in the kernel data or bss section */
	if ((addr >= (void *)&_sdata && addr <= (void *)&_edata) || (addr >= (void *)&_sbss && addr <= (void *)&_ebss)) {
		return true;
	}
	return false;
}

#ifdef CONFIG_BUILD_PROTECTED
bool is_kernel_space(void *addr)
{
	if (is_kernel_text_space(addr) || is_kernel_data_space(addr)) {
		return true;
	}
	return false;
}
#endif							/* CONFIG_BUILD_PROTECTED */
