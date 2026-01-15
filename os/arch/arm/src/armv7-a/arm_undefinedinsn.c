/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/armv7-a/arm_undefinedinsn.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/arch.h>

#ifdef CONFIG_SYSTEM_REBOOT_REASON
#include <tinyara/reboot_reason.h>
#include <arch/reboot_reason.h>
#endif

#include "up_internal.h"

/****************************************************************************
 * Public Data
 ****************************************************************************/
extern uint32_t system_exception_location;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: print_undefinedinsn_detail
 ****************************************************************************/

static inline void print_undefinedinsn_detail(uint32_t *regs)
{
	/* Abort log must always start at a new line.*/
	lldbg_noarg("\n");
	_alert("#########################################################################\n");
	_alert("PANIC!!! Undefined instruction at 0x%x\n", regs[REG_PC]);
	_alert("#########################################################################\n\n\n");
}

/****************************************************************************
 * Name: arm_undefinedinsn
 ****************************************************************************/

uint32_t *arm_undefinedinsn(uint32_t *regs)
{
	/* Save the saved processor context in CURRENT_REGS where it can be
	 * accessed for register dumps and possibly context switching.
	 */
	uint32_t *saved_state = (uint32_t *)CURRENT_REGS;
	CURRENT_REGS = regs;
	system_exception_location = regs[REG_R15];
#ifdef CONFIG_SYSTEM_REBOOT_REASON
	up_reboot_reason_write(REBOOT_SYSTEM_PREFETCHABORT);
#endif

	/* Crash -- possibly showing diagnostic debug information. */
	if (!IS_SECURE_STATE()) {
		print_undefinedinsn_detail(regs);
	}

	PANIC();
	regs = (uint32_t *)CURRENT_REGS;
	CURRENT_REGS = saved_state;
	return regs; /* To keep the compiler happy */
}
