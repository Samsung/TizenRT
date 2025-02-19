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

/***************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <tinyara/kmalloc.h>
#include <tinyara/security_hal.h>
#include <tinyara/seclink_drv.h>
#include <tinyara/ss_slot_index.h>
#include <tinyara/security_level.h>

/***************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private data
 ****************************************************************************/
/* Default security_level value: high */
static int security_level = HIGH_SECURITY_LEVEL;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: set_security_level
 *
 * Description:
 *   This function read the level of security from secure storage and write it
 *   in 'security_level' variable.
 *   'security_level' variable is defined as static type so that no one can
 *   modify the value by approaching it carelessly.
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/
int set_security_level(void)
{
	struct sec_lowerhalf_s *se = (struct sec_lowerhalf_s *)se_get_device();
	hal_data output;
	hal_result_e hal_ret;

	output.priv_len = 0;
	output.priv = NULL;
	output.data_len = 2;
	output.data = (char *)kmm_malloc(output.data_len);

	if (output.data == NULL) {
		dbg("Alloc fail. Security level is high\n");
		return SECURITY_LEVEL_ALLOC_FAILED;
	}

	hal_ret = se->ops->read_storage(SS_SLOT_INDEX_SECURITY_LEVEL, &output);
	if (hal_ret != HAL_SUCCESS) {
		kmm_free(output.data);
		dbg("Not set. Security level is high\n");
		return SECURITY_LEVEL_READ_FAILED;
	}

	int result = atoi(output.data);
	if (result == HIGH_SECURITY_LEVEL) {
		security_level = HIGH_SECURITY_LEVEL;
		dbg("HIGH\n");
	} else if (result == LOW_SECURITY_LEVEL) {
		security_level = LOW_SECURITY_LEVEL;
		dbg("LOW\n");
	} else {
		kmm_free(output.data);
		dbg("Wrong value!! Using sec_level command to set\n");
		return SECURITY_LEVEL_INVALID_VALUE;
	}

	kmm_free(output.data);

	return SECURITY_LEVEL_OK;
}

/****************************************************************************
 * Name: get_security_level
 *
 * Description:
 *   This function returns the security_level.
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   0(low) or 1(high)
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/
int get_security_level(void)
{
	return LOW_SECURITY_LEVEL;
}
