/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 *
 *   Copyright (C) 2020 Gregory Nutt. All rights reserved.
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

#include <stdbool.h>
#include <debug.h>
#include <tinyara/reboot_reason.h>
#include "armino_reboot_reason.h"
/****************************************************************************
 * Public functions
 ****************************************************************************/
#ifdef CONFIG_SYSTEM_REBOOT_REASON

static reboot_reason_code_t reboot_reason;

static reboot_reason_code_t up_reboot_reason_get_hw_value(void)
{
	uint32_t boot_reason = 0;

	/* Read the same backup register for the boot reason */
	boot_reason = bk_misc_get_reset_reason();
    rrvdbg("boot_reason = %d\n", boot_reason);

	/* Check if this is a software-written reboot reason (value >= 50) */
	if (boot_reason >= REBOOT_REASON_INITIALIZED) {
		return boot_reason;
	}

	/* Hardware reset reason, need to convert to software layer value */
	switch (boot_reason) {
		/* HW reset */
		case RESET_SOURCE_POWERON:
			return REBOOT_SYSTEM_HW_RESET;

		/* Soft reset */
		case RESET_SOURCE_REBOOT:
			return REBOOT_SYSTEM_HW_RESET;

		/* WDT reset - hardware layer value */
		case RESET_SOURCE_WATCHDOG:
			return REBOOT_SYSTEM_WATCHDOG;

		/* WDT reset - NMI interrupt */
		case RESET_SOURCE_NMI_WDT:
			return REBOOT_SYSTEM_WATCHDOG;

		default:
			return REBOOT_UNKNOWN;
	}
}

void up_reboot_reason_init(void)
{
	reboot_reason = up_reboot_reason_get_hw_value();
    bk_misc_set_reset_reason(REBOOT_REASON_INITIALIZED);
}

reboot_reason_code_t up_reboot_reason_read(void)
{
	int reason = reboot_reason;
	rrvdbg("Read Reboot Reason : %d\n", reason);
	return reason;
}

void up_reboot_reason_write(reboot_reason_code_t reason)
{
	rrvdbg("Write Reboot Reason : %d\n", reason);
	bk_misc_set_reset_reason((uint32_t)reason);
}

void up_reboot_reason_clear(void)
{
	rrvdbg("Clear Reboot Reason\n");
	/* Reboot Reason Clear API writes the REBOOT_REASON_INITIALIZED by default.
	 * If chip vendor needs another thing to do, please change the below.
	 */
	up_reboot_reason_write(REBOOT_UNKNOWN);
	reboot_reason = REBOOT_UNKNOWN;
}

bool up_reboot_reason_is_written(void)
{
	if (aon_pmu_hal_get_reset_reason() != REBOOT_REASON_INITIALIZED) {
		return true;
	}

	return false;
}
#endif
