/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>

#if defined(CONFIG_ARMV8M_MPU) && defined(CONFIG_BUILD_PROTECTED)
#include <tinyara/mpu.h>
#include "up_mpuinit.h"
#endif

#include "up_internal.h"

extern uint32_t _sbss;
extern uint32_t _ebss;
extern const uint32_t _eronly;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sidle_stack;

const uintptr_t g_idle_topstack =
	(uintptr_t)&_sidle_stack + CONFIG_IDLETHREAD_STACKSIZE;

#ifdef CONFIG_DEBUG
#define showprogress(c) up_lowputc(c)
#else
#define showprogress(c)
#endif

void __start(void)
{
	uint32_t *dest;

	showprogress('A');

	for (dest = &_sbss; dest < &_ebss;) {
		*dest++ = 0;
	}

#if defined(CONFIG_XIP_KERNEL) || defined(CONFIG_BOOT_RUNFROMFLASH)
	{
		const uint32_t *src;

		for (src = &_eronly, dest = &_sdata; dest < &_edata;) {
			*dest++ = *src++;
		}
	}
#endif

#ifdef USE_EARLYSERIALINIT
	up_earlyserialinit();
#endif

	showprogress('B');

#if defined(CONFIG_ARMV8M_MPU) && defined(CONFIG_BUILD_PROTECTED)
	mpu_set_nregion_board_specific(0);
	up_mpuinitialize();
#endif

	showprogress('C');

#ifdef CONFIG_STACK_COLORATION
	go_os_start((void *)&_sidle_stack, CONFIG_IDLETHREAD_STACKSIZE);
#else
	os_start();
#endif

	for (;;) {
	}
}
