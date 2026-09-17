/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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
#include <sys/types.h>
#include <debug.h>
#include <tinyara/binary_manager.h>

#include "binary_manager_internal.h"

/****************************************************************************
 * Name: binary_manager_early_initialize
 *
 * Description:
 *   Check boot parameters and mount resources after all partitions have
 *   been registered and before boot trial processing begins.
 *
 ****************************************************************************/
void binary_manager_early_initialize(void)
{
#ifdef CONFIG_USE_BP
	int ret;
	uint8_t recovery_reason;

	/* Check binary set alignment early and recover it if necessary. */
	ret = binary_manager_check_bootparam_set();
	if (ret != OK) {
		recovery_reason = binary_manager_get_bp_recovery_reason();
		ret = binary_manager_recover_bootparam_set(recovery_reason);
		if (ret != OK) {
			lldbg("ERROR: Failed to recover bootparam set, ret %d\n", ret);
			return;
		}
	}
#endif

#ifdef CONFIG_RESOURCE_FS
	/* Mount resources early because audio and LCD initialization may need them. */
	if (binary_manager_mount_resource() != OK) {
		lldbg("ERROR: Failed to mount resource\n");
	}
#endif
}
