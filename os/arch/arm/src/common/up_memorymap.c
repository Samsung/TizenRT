/****************************************************************************
 *
 * Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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
#include <tinyara/mm/mm.h>
#include <tinyara/mm/kasan.h>
#include <tinyara/arch.h>
#include <debug.h>

#include "up_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Type Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_memorymap
 *
 * Description:
 *  Print memory map
 *
 * Input Parameters:
 *  heap_start - first address of heap region
 *  heap_size  - size of a heap
 *
 * Returned Value:
 *  None
 *
 * Assumption:
 *  None
 *
 ****************************************************************************/
void up_memorymap(void *heap_start, size_t heap_size)
{
	mllvdbg("----------data------------\n");
	mllvdbg("_sdata: 0x%lx\n", &_sdata);
	mllvdbg("_edata: 0x%lx\n", &_edata);
	mllvdbg("data size: %d\n",
		(unsigned long)&_edata - (unsigned long)&_sdata);
	mllvdbg("----------bss------------\n");
	mllvdbg("_sbss: 0x%lx\n", &_sbss);
	mllvdbg("_ebss: 0x%lx\n", &_ebss);
	mllvdbg("bss size: %d\n",
		(unsigned long)&_ebss - (unsigned long)&_sbss);
	mllvdbg("----------stack------------\n");
	mllvdbg("IDLE_STACK_BASE: 0x%lx\n",
		(unsigned long)g_idle_topstack -
		CONFIG_IDLETHREAD_STACKSIZE);
	mllvdbg("g_idle_topstack: 0x%lx\n", g_idle_topstack);
	mllvdbg("IDLETREAD_STACKSIZE: %d\n", CONFIG_IDLETHREAD_STACKSIZE);
	mllvdbg("----------heap------------\n");
	mllvdbg("heap_start: 0x%lx\n", (unsigned long)heap_start);
	mllvdbg("heap_end: 0x%lx\n",
		(unsigned long)heap_start + heap_size);
	mllvdbg("heap_size: %d\n", heap_size);
#ifdef CONFIG_KASAN
	mllvdbg("----------shadow------------\n");
	mllvdbg("SHADOW_START: 0x%lx\n", KASAN_SHADOW_START);
	mllvdbg("SHADOW_END: 0x%lx\n", KASAN_SHADOW_END);
	mllvdbg("SHADOW_SIZE: %d\n", KASAN_SHADOW_SIZE);
	mllvdbg("SHADOW_OFFSET: 0x%lx\n", KASAN_SHADOW_OFFSET);
#endif
	mllvdbg("--------------------------\n");
	mllvdbg("CONFIG_RAM_START: 0x%lx\n", CONFIG_RAM_START);
	mllvdbg("CONFIG_RAM_END: 0x%lx\n", CONFIG_RAM_END);
	mllvdbg("CONFIG_RAM_SIZE: %d\n", CONFIG_RAM_SIZE);
	mllvdbg("--------------------------\n");
	mllvdbg("SIZEOF_MM_ALLOCNODE: %d\n", SIZEOF_MM_ALLOCNODE);
	mllvdbg("SIZEOF_MM_FREENODE: %d\n", SIZEOF_MM_FREENODE);
}
