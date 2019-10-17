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

#include "up_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef CONFIG_BUILD_PROTECTED

bool is_kernel_space(void *addr)
{
	uint32_t ksram_segment_end  = (uint32_t)__ksram_segment_start__  + (uint32_t)__ksram_segment_size__;
	uint32_t kflash_segment_end = (uint32_t)__kflash_segment_start__ + (uint32_t)__kflash_segment_size__;

	if ((addr >= (void *)__ksram_segment_start__ && addr <= (void *)ksram_segment_end) || (addr >= (void *)__kflash_segment_start__ && addr < (void *)kflash_segment_end)) {
		return true;
	}
	return false;
}

#endif							/* CONFIG_BUILD_PROTECTED */
