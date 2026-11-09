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

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <debug.h>
#include <string.h>

#include <tinyara/reboot_reason.h>

#include "binary_manager_internal.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: binary_manager_trial_rollback
 *
 * Description:
 *   Update the BP to select the set opposite to its active kernel slot,
 *   clear the trial count and record the rollback reason. Save the BP to
 *   the other BP copy, then reboot with REBOOT_SYSTEM_BINARY_ROLLBACK.
 *
 ****************************************************************************/
int binary_manager_trial_rollback(binmgr_bpdata_t *bp_data)
{
	int ret;
	uint8_t target_idx;
#ifdef CONFIG_APP_BINARY_SEPARATION
	uint32_t app_idx;
#endif

	/* Select the opposite kernel, app and resource set. */
	target_idx = bp_data->head.active_idx ^ 1;

	bp_data->head.active_idx = target_idx;
#ifdef CONFIG_APP_BINARY_SEPARATION
	for (app_idx = 0; app_idx < bp_data->head.app_count; app_idx++) {
		bp_data->head.app_data[app_idx].useidx = target_idx;
	}
#endif
#ifdef CONFIG_RESOURCE_FS
	bp_data->head.resource_active_idx = target_idx;
#endif
	/* Clear the trial count so the selected slot skips boot trials. */
	bp_data->tail.try_count = 0;
	bp_data->tail.bp_update_reason = BP_UPDATE_BINARY_MANAGER_ROLLBACK;

	ret = binary_manager_commit_bootparam(bp_data);
	if (ret != BINMGR_OK) {
		return ret;
	}

	bmdbg("Trial rollback to slot %u\n", bp_data->head.active_idx);
	binary_manager_reset_board(REBOOT_SYSTEM_BINARY_ROLLBACK);

	/* A successful reset never returns. Do not continue with the updated
	 * BP while the current image is still executing.
	 */
	return BINMGR_OPERATION_FAIL;
}

/****************************************************************************
 * Name: binary_manager_trial_boot_start
 *
 * Description:
 *   Read the BP and skip trial processing if the count is zero. Otherwise,
 *   increment and save the count before continuing initialization. If five
 *   attempts were recorded without confirmation, roll back on this call.
 *
 ****************************************************************************/
int binary_manager_trial_boot_start(void)
{
	int ret;
	binmgr_bpdata_t bp_data;

	ret = binary_manager_update_bpinfo();
	if (ret != BINMGR_OK) {
		bmdbg("Failed to read BP for trial boot, ret %d\n", ret);
		return ret;
	}

	memcpy(&bp_data, binary_manager_get_bpdata(), sizeof(bp_data));
	if (bp_data.tail.try_count == 0) {
		bmvdbg("Normal booting\n");
		return BINMGR_OK;
	}

	if (bp_data.tail.try_count > BOOTPARAM_TRIAL_MAX) {
		bmdbg("Trial boot limit exceeded, count %u, max %u. Rolling back\n", bp_data.tail.try_count, BOOTPARAM_TRIAL_MAX);
		ret = binary_manager_trial_rollback(&bp_data);
		if (ret != BINMGR_OK) {
			bmdbg("Trial rollback failed. ret %d\n", ret);
		}
		return ret;
	}

	bmdbg("Trial boot attempt %u/%u\n", bp_data.tail.try_count, BOOTPARAM_TRIAL_MAX);
	bp_data.tail.try_count++;

	ret = binary_manager_commit_bootparam(&bp_data);
	if (ret != BINMGR_OK) {
		bmdbg("Failed to save trial boot count %u, ret %d\n", bp_data.tail.try_count, ret);
	}

	return ret;
}

/****************************************************************************
 * Name: binary_manager_trial_boot_confirm
 *
 * Description:
 *   Clear and save the trial count at the OS initialization checkpoint,
 *   before creating appinit, the binary manager or appmain. A zero count
 *   returns without writing the BP.
 *
 ****************************************************************************/
int binary_manager_trial_boot_confirm(void)
{
	binmgr_bpdata_t bp_data;

	/* Reuse the BP cache updated by trial start. The startup sequence has
	 * not created appinit, the binary manager or appmain yet.
	 */
	memcpy(&bp_data, binary_manager_get_bpdata(), sizeof(bp_data));
	if (bp_data.tail.try_count == 0) {
		return BINMGR_OK;
	}

	bp_data.tail.try_count = 0;
	return binary_manager_commit_bootparam(&bp_data);
}
