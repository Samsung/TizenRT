/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>
#include <arch/reboot_reason.h>
#include <tinyara/arch.h>
#include <tinyara/reboot_reason.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: reboot_reason_try_write_assert
 ****************************************************************************/
#ifdef CONFIG_SYSTEM_REBOOT_REASON
void reboot_reason_try_write_assert(uintptr_t addr)
{
	/* Write reboot reason only when there is no written reason before. */
	if (!up_reboot_reason_is_written()) {
		up_reboot_reason_write_by_addr(addr, REBOOT_SYSTEM_ASSERT);
	}
}

/****************************************************************************
 * Name: up_reboot_reason_write_by_addr
 ****************************************************************************/
void up_reboot_reason_write_by_addr(uintptr_t addr, reboot_reason_code_t reason)
{
#ifdef CONFIG_APP_BINARY_SEPARATION
	if (!is_kernel_space((void *)addr)) {
		switch (reason) {
		case REBOOT_SYSTEM_DATAABORT:
			reason = REBOOT_USER_DATAABORT;
			break;
		case REBOOT_SYSTEM_PREFETCHABORT:
			reason = REBOOT_USER_PREFETCHABORT;
			break;
		case REBOOT_SYSTEM_MEMORYALLOCFAIL:
			reason = REBOOT_USER_MEMORYALLOCFAIL;
			break;
		case REBOOT_SYSTEM_WATCHDOG:
			reason = REBOOT_USER_WATCHDOG;
			break;
		case REBOOT_SYSTEM_ASSERT:
			reason = REBOOT_USER_ASSERT;
			break;
		case REBOOT_SYSTEM_WITHOUT_SET_REASON:
			reason = REBOOT_USER_WITHOUT_SET_REASON;
			break;
		default:
			break;
		}
	}
#endif

	up_reboot_reason_write(reason);
}
#endif
